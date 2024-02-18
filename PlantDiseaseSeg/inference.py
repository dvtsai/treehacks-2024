import torch
from torchvision import models
import cv2   


# real time inference 

# define a video capture object 
vid = cv2.VideoCapture(0) 
  
while(True): 
      
    # Capture the video frame 
    # by frame 
    ret, img = vid.read() 

    # give img to model
    # model = models.mobilenet_v2(pretrained=True).eval()

    # load using weights gained after training 
    model = createDeepLabv3(pretrained=False)
    model.load_state_dict(torch.load("best_10_epoch.pth"))
    model.eval()

    # Preprocess the image
    img = cv2.resize(img, (128,128))
    # Add a batch dimension
    img = img.unsqueeze(0)

    # Perform inference
    with torch.no_grad():
        outputs = model(img)
        _, predicted = outputs.max(1)
        label = classes[predicted.item()]

    # Annotate and display the frame
    cv2.putText(img, label, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
    cv2.imshow('frame', frame)
  
    # Display the resulting frame 
    cv2.imshow('frame', img) 
      
    # the 'q' button is set as the 
    # quitting button you may use any 
    # desired button of your choice 
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
  
# After the loop release the cap object 
vid.release() 
# Destroy all the windows 
cv2.destroyAllWindows() 


