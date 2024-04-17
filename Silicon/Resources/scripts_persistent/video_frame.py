import sys
import cv2
import os

file_path = sys.argv[1]
destination_path = sys.argv[2]
file_size = int(sys.argv[3])

vidcap = cv2.VideoCapture(file_path)
vidcap.set(cv2.CAP_PROP_FPS,30)
success,image = vidcap.read()

count = 0
while success:
      if(file_size > 100000000):
        count = count + 1
        if(count == 30):
           cv2.imwrite(destination_path,image)
           break
        success,image = vidcap.read()
      else:
        cv2.imwrite(destination_path,image)
        break
        
cv2.destroyAllWindows()

