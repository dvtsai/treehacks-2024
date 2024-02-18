import cv2
import torch
from torchvision import models


cap = cv2.VideoCapture(1)
cap.set(3, 640)
cap.set(4, 480)


model = models.efficientnet_v2_s(pretrained = False)

# mobile_net = torchvision.models.mobilenet_v2(weights="MobileNet_V2_Weights.DEFAULT").features
# mobile_net.eval() 

model.load_state_dict(torch.load('epoch9.pth'), strict = False)


while True:
    ret, img= cap.read()
    cv2.imshow('Webcam', img)
    img_resized = cv2.resize(img, (128, 128))

    results = model(img_resized, stream=True)


    if cv2.waitKey(1) == ord('q'):
        break



cap.release()
cv2.destroyAllWindows()