import cv2
from grip import GripPipeline
from networktables import NetworkTables
import numpy as np
import os, time

NetworkTables.initialize(server='10.42.37.2')
table = NetworkTables.getTable('GRIP/vision')
idNum = 0

def extra_processing(pipeline):
    global idNum
    global newLogName
    buff = []
    xsum = 0

    # Find the bounding boxes of the contours to get x, y, width, and height
    
    for contour in pipeline.filter_contours_output:
        x, y, w, h = cv2.boundingRect(contour)
        buff.append(x + w / 2.0)
        buff.append(h)
        xsum += (x + w / 2.0) + (h)

    idNum += 1
    xsum += idNum

    buff.append(idNum)
    buff.append(xsum)

    try:
        table.putNumberArray('Pi TargetInfo', buff)

    except Exception as ex:
        print "Exception writing to NetworkTables:"
        print str(ex)

    print str(buff)


def main():
    global idNum
    while True:
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
                    camTable = NetworkTables.getTable('CameraPublisher/USB Camera 0')
                    cameras = camTable.getStringArray('streams')
                    print cameras
                    return
#                    for item in cameras:
#                        print item
                    stream = cv2.VideoCapture('http://10.42.37.2:1181/stream.mjpg')
                    print "Stream Opened"
                    didStreamSucceed = True

                except Exception as err:
                    print "Error creating stream: " + str(err)

        print "Initialization Complete"

        while stream.isOpened():
                ret, img = stream.read()
                if ret is True:
                    pipeline.process(img)
                    if len(pipeline.filter_contours_output) >= 1 and (idNum % 10) is 0:
                        print "Writing image to usb"
                        cv2.imwrite("/mnt/usb/" + str(idNum) + "raw.jpg", img)
                        cv2.imwrite("/mnt/usb/" + str(idNum) + "filtered.jpg", pipeline.hsl_threshold_output)
                    extra_processing(pipeline)
                else:
                    print "Error reading from camera"
                    break
if __name__ == '__main__':
    main()
