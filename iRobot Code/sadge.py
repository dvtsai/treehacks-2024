import cv2
import torch
from torchvision import models
from torchvision.transforms import functional as F

def load_model(checkpoint_path):
    # Initialize the model
    model = models.efficientnet_v2_s(pretrained=False)  # Avoid using deprecated arguments

    # Attempt to load the state dict with error handling
    state_dict = torch.load(checkpoint_path)
    try:
        model.load_state_dict(state_dict)
    except RuntimeError as e:
        print(f"Error loading state dict: {e}")
        # Handle missing keys, unexpected keys, etc.

    model.eval()
    return model

def preprocess_image(image, target_size=(128, 128)):
    # Convert the color space from BGR to RGB
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Resize and convert to PIL Image
    image = cv2.resize(image, target_size)
    image = F.to_pil_image(image)

    # Apply standard transformations
    image = F.to_tensor(image)
    image = F.normalize(image, mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    return image.unsqueeze(0)  # Add batch dimension

def main():
    model = load_model('epoch9.pth')  # Specify the path to your .pth file

    cap = cv2.VideoCapture(1)  # Initialize the webcam
    while True:
        ret, frame = cap.read()  # Read a frame
        if not ret:
            break

        # Preprocess the image
        input_tensor = preprocess_image(frame)

        # Perform inference
        with torch.no_grad():
            outputs = model(input_tensor)

        with open("imagenet_classes.txt") as f:
            classes = [line.strip() for line in f.readlines()]

        _, predicted = outputs.max(1)
        label = classes[predicted.item()]

        for r in outputs:
            boxes = r.boxes

            for box in boxes:
                # bounding box
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2) # convert to int values

                # put box in cam
                cv2.rectangle(img, (x1, y1), (x2, y2), (255, 0, 255), 3)

                # confidence
                confidence = math.ceil((box.conf[0]*100))/100
                print("Confidence --->",confidence)

                # class name
                cls = int(box.cls[0])
                print("Class name -->", classNames[cls])

                # object details
                org = [x1, y1]
                font = cv2.FONT_HERSHEY_SIMPLEX
                fontScale = 1
                color = (255, 0, 0)
                thickness = 2

                cv2.putText(frame, label, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

        # Post-processing (if necessary) and display the frame
        # For demonstration, we'll just display the original frame
        cv2.imshow('Webcam', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):  # Exit on 'q' key press
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
