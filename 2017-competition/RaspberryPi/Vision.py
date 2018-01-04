import cv2
from grip import GripPipeline
from networktables import NetworkTables
import numpy as np
import urllib

NetworkTables.initialize(server='10.42.37.2')
table = NetworkTables.getTable('GRIP/vision')
idNum = 0

def extra_processing(pipeline):

    """
    Performs extra processing on the pipeline's outputs and publishes data to NetworkTables.
    :param pipeline: the pipeline that just processed an image
    :return: None
    """

    global idNum 
    buff = []
    xsum = 0

    # Find the bounding boxes of the contours to get x, y, width, and height

    for contour in pipeline.filter_contours_output:
        x, y, w, h = cv2.boundingRect(contour)
        buff.append ( x + w / 2.0 )
        buff.append (  h )
        xsum += ( x + w / 2.0 ) + ( h )

    idNum += 1
    xsum += idNum

    buff.append ( idNum )
    buff.append ( xsum )

    try:
        table.putNumberArray( 'TargetInfo', buff )

    except Exception as ex:
        print "Exception writing to ntTables:"
        print str(ex)

    print str(buff)


def main():

    global idNum
    plDone = False
    didStreamSucceed = False

    while didStreamSucceed is False:

        if plDone is False:
            try:       
                print "Creating pipeline"
                pipeline = GripPipeline.GripPipeline()
                print "Pipeline created"
                plDone = True

            except Exception as err:
                print "Error creating pipeline: " + str(err)

        if plDone is True:
            try:
                print "Opening Stream"
                stream = urllib.urlopen('http://10.42.37.2:1181/?action=stream')
                print "Stream Opened"
                didStreamSucceed = True

            except Exception as err:
                print "Error creating stream: " + str(err)

    print "Initialization Complete"
    bytes = ''

    isRunning = True

    while isRunning is True:

        try:
            bytes += stream.read(16384)
            b = bytes.rfind('\xff\xd9')
            a = bytes.rfind('\xff\xd8', 0, b-1)

            if a != -1 and b != -1:
                jpg = bytes[a:b+2]
                bytes = bytes[b+2:]
                img = cv2.imdecode(np.fromstring(jpg, dtype = np.uint8), cv2.IMREAD_COLOR)
                #print "Writing /mnt/usb/pre" + str(idNum) + ".jpg"
                #cv2.imwrite("/mnt/usb/pre" + str(idNum) + ".jpg", img)
                #print "Running"
                pipeline.process(img, idNum)
                extra_processing(pipeline)

        except KeyboardInterrupt:
            print "Ending"
            isRunning = False

        except Exception as err:
            print "Error in main loop, continuing: " + str(err)


if __name__ == '__main__':
    main()
