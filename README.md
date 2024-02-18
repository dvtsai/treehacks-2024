# TreeHacks 2024: Autonomous Agricultural Robot for Real-Time Plant Monitoring

## Inspiration
Our inspiration for creating an autonomous agricultural robot stemmed from the need for more efficient and precise agricultural practices in an industry that does not have enough resources. Specifically, plant monitoring is important to maximize crop yield and prevent plant viruses. Automating the plant monitoring process will allow farmers to dedicate their time to other tasks while increasing the efficiency of plant monitoring operations. Companies like John Deere, have already invested in automated machinery to increase the efficiency of their products, highlighting the importance of autonomous robotics solutions in the farming industry. We aimed to design a robot that is capable of real-time plant monitoring to cut off diseased plants using computer vision. 

## What it does
Our robot arm is equipped with claws at its end to simulate scissors. A green disk is used to simulate the localization of the robot by accurately cutting once it senses the disk is in the center. This behavior is controlled by a laptop camera, which gives the robot arm data to process. It is also equipped with a moisture sensor and lights that change color once the sensor detects a change in moisture. 

## How we built it
Our robot employs state-of-the-art computer vision techniques for real-time plant monitoring through image classification and semantic segmentation by building off of the baseline EfficientNetV2 and DeepLabV3 models. It can classify various plant species encountered in its environment and detect plant diseases through identifying marks on leaves. Baseline deep learning models were modified through machine learning techniques like fine-tuning and transfer learning. We optimized the models on an Intel CPU using the Intel Developer Cloud. The hardware consisted of a robot arm designed with CAD, laser cutting, 3D printing, and soldering. 

## Challenges we ran into
On the software side, we faced a significant challenge in fine-tuning our models due to the scarcity of high-quality datasets for plant diseases. Hardware presented its own set of challenges, particularly ensuring that the robot arm was structurally sound enough to support its own weight.

## Accomplishments that we're proud of
We are proud of our achievements in the hardware-software integration in the robot arm. Within 36 hours, we were able to design and build a robot arm, train the appropriate computer vision models for plant monitoring, and put our models onto the robot arm. 

## What we learned
We started off on Friday night without a project idea in mind. After many hours of ideation and brainstorming, we finalized on the idea of agricultural robots because this was a field with demand that we could tap into. We learned a lot about the iterative design process, including the initial customer discovery process, developing our business model and pitch, and the technical implementations needed for our idea. We learned how interdisciplinary the field of robotics is- in our team of four, we have a robot perception engineer, a machine learning engineer, a mechatronics engineer, and a mechanical engineer. By leveraging all of our strengths, we were able to deploy our initial software onto a robot arm we designed. On the technical side, we learned the intricacies of fine-tuning deep learning models for transfer learning, the importance of robust dataset collection, and the complexities involved in hardware design, fabrication, and its integration with software. 

## What's next 
Future work includes improving the robot's decision-making capabilities, enhancing its physical design for better efficiency, and expanding its functionalities to cater to a broader range of agricultural tasks. Work can also be done to make the robot fully autonomous by integrating GNC and SLAM solutions to help it generalize to different types of farming environments. 
