# You Only Look Once: Unified, Real-Time Object Detection 논문 리뷰

### Abstract

Object Detection 대한 새로운 접근 방식인 YOLO: 1-stage detector로 특징 분류와 classification을 regression문제로 frame화 함, 매우 빠름

### 1. Introduction

기존 detection 모델은 classfier를 재정의하여 detector로 사용

classification이란 하나의 이미지를 보고 그것이 개인지 고양이인지 판단하는 것

하지만 object detection은 하나의 이미지 내에서 개는 어디에 위치해 있고, 고양이는 어디에 위치해 있는지 판단하는 것 → 따라서 object detection은 classification 뿐만 아니라 위치 정보도 판단해야함 (DPM, R-CNN)

DPM은 이미지 전체를 sliding window로 object detection을 진행, R-CNN은 이미지 안에서 bounding box를 생성하기 위해 region proposal을 하고, 제안된 bounding box에 classifier를 적용하여 classification을 진행, 그 다음 bounding box를 조정하고, 중복된 검출을 제거하고, 객체에 따라 box의 점수를 재산정하기 위해 post-processing을 진행 → 이런 복잡함 때문에 R-CNN은 느리고 각 절차를 독립적으로 훈련시켜야 해서 optimization에 어려움

그래서, object detection을 single regression problem으로 재정의: 이미지의 픽셀로부터 bounding box의 위치(coordinates), 클래스 확률(class probabilities)을 구하기까지의 일련을 절차를 하나의 회귀 문제로 재정의

이미지를 한 번만 보면 객체를 검출할 수 있다 하여 이름이 YOLO(you only look once)

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled.png)

→하나의 convolutional network가 여러 bounding box와 그 bounding box의 클래스 확률을 동시에 계산, YOLO는 이미지 전체를 학습하여 곧바로 detection performance을 최적화

YOLO의 이런 통합된 모델은 기존의 객체 검출 모델에 비해 여러 가지 장점이 존재

1. YOLO는 굉장히 빠름: 왜냐하면 YOLO는 기존의 복잡한 object detection process를 single regression problem 바꾸었기 때문
2. YOLO는 예측을 할 때 이미지 전체를 사용: sliding window나 region proposal 방식과 달리, YOLO는 훈련과 테스트 단계에서 이미지 전체를 보고, 그래서 클래스의 모양에 대한 정보뿐만 아니라 주변 정보까지 학습하여 처리 → 아무 물체가 없는 background에 반점이나 노이즈가 있으면 그것을 물체로 인식하는 background error가 Fast R-CNN에 비해 적음
3. YOLO는 물체의 일반적인 부분을 학습: 다른 모델에 비해 YOLO는 훈련 단계에서 보지 못한 새로운 이미지에 대해 더 robust함

하지만, YOLO는 빠르게 객체를 검출할 수 있다는 장점은 있지만 정확성이 다소 떨어짐

### 2. Unified Detection

YOLO는 input images를 S x S grid로 나눔: 만약 어떤 객체의 중심이 특정 grid cell 안에 위치한다면, 그 그리드 셀이 해당 객체를 검출해야 함

각각의 grid cell은 B개의 bounding box와 그 bounding box에 대한 confidence score를 예측

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%201.png)

IOU(intersection over union)는 실제 bounding box와 예측 bounding box의 합집합 면적 대비 교집합 면적의 비율: IOU = (실제 bounding box와 예측 bounding box의 교집합) / (실제 bounding box와 예측 bounding box의 합집합)

그리드 셀에 아무 객체가 없다면 Pr(Obejct)=0이고, confidence score도 0 , 그리드 셀에 어떤 객체가 확실히 있다고 예측했을 때, 즉 Pr(Object)=1일 때가 가장 이상적

→ 따라서 confidence score가 IOU와 같다면 가장 이상적인 score

각각의 bounding box는 5개의 예측치로 구성(x, y, w, h, confidence): (x, y) 좌표 쌍은 bounding box 중심의 그리드 셀(grid cell) 내 상대 위치(0~1 사이의 값) / (w, h) 쌍은 bounding box의 상대 너비와 상대 높이(0~1 사이의 값) / confidence=confidence score(Pr*IOU)

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%202.png)

각각의 grid cell은 C(conditional class)를 예측

→ 그리드 셀 안에 객체가 있다는 조건 하에 그 객체가 어떤 class인지에 대한 조건부 확률: 그리드 셀에 몇 개의 bounding box가 있는지와는 무관하게 하나의 그리드 셀에는 오직 하나의 class에 대한 probability만 구함

테스트 단계에서는 conditional class probability(C)와 개별 bounding box의 confidence score를 곱해주는데, 이를 각 bounding box에 대한 class-specific confidence score라고 함

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%203.png)

→ class-specific confidence score는 conditional class probability와 confidence score를 곱한 값

이 score는 bounding box에 특정 class 객체가 나타날 확률(=Pr(Class_i))과 예측된 bounding box가 그 클래스(class) 객체에 얼마나 잘 들어맞는지(=IOU_pred^truth)를 나타냄

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%204.png)

**2.1. Network Design**

YOLO의 신경망 구조는 image classification에 사용되는 GoogLeNet에서 따옴: 총 24개의 convolutional layers과 2개의 fully connected layers으로 구성되어 있고, GoogLeNet의 인셉션 구조 대신 YOLO는 1 x 1 reduction layer과 3 x 3 convolution layer 결합을 사용

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%205.png)

→ 최종 output은 7 x 7 x 30의 prediction tensors

**2.2. Training**

1000개의 class를 갖는 ImageNet 데이터 셋으로 YOLO의 convolution layer을 pretrain: pretrain을 위해서 24개의 convolution layer 중 첫 20개의 convolution layer만 사용했고, 이어서 average-pooling layer과 fully connectied layer을 연결 → ImageNet 2012 validation 데이터 셋에서 88%의 정확도 기록

ImageNet은 classification를 위한 데이터 셋이어서, object detection 모델로 바꾸어야 함 → pretrain된 20개의 convolution layer 뒤에 4개의 convolution layer 및 2개의 fully connected layer를 추가해 성능을 향상시켰고 입력 이미지의 해상도를 224 x 224에서 448 x 448로 증가시킴

이 신경망의 최종 output은 class probabilities과 bounding box 위치정보(coordinates)

YOLO 신경망의 마지막 계층에는 linear activation function를 적용했고, 나머지 모든 계층에는 leaky ReLU를 적용

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%206.png)

YOLO의 loss는 SSE(sum-squared error)를 기반으로 해서, 최종 output의 SSE를 optimize 해야함

하지만, SSE를 최적화하는 것이 YOLO의 최종 목적인 mAP(평균 정확도)를 높이는 것과 완벽하게 일치하지 않음: bounding box의 위치를 얼마나 잘 예측했는지에 대한 loss인 localization loss와 class를 얼마나 잘 예측했는지에 대한 loss인 classification loss가 있는데, 두 loss의 가중치를 동일하게 두고 학습시키는 것은 좋은 학습이 아님

또 다른 문제가 있는데, 배경 영역이 전경 영역보다 더 크기 때문에 그리드 셀에 객체가 없다면 confidence score=0이고, 대부분의 그리드 셀의 confidence score=0이 되게 학습할 수 밖에 없고 이는 모델 불균형을 초래

이를 개선하기 위해, 객체가 존재하는 bounding box 좌표(coordinate)에 대한 loss의 가중치를 증가시키고, 객체가 존재하지 않는 bounding box의 confidence loss에 대한 가중치는 감소: 이를 위해 두 개의 파라미터를 사용했는데, λ_coord와 λ_noobj이며, λ_coord=5, λ_noobj=0.5로 가중치 셋팅

또 다른 문제로, SSE는 큰 bounding box와 작은 boudning box에 대해 모두 동일한 가중치로 loss를 계산하지만 작은 bounding box가 큰 bounding box보다 작은 위치 변화에 더 민감함 → 이를 개선하기 위해 bounding box의 width와 height에 square root를 취함: 너비와 높이가 커짐에 따라 그 증가율이 감소해 loss에 대한 가중치를 감소시키는 효과

YOLO는 하나의 grid cell에 여러 개의 bounding box를 예측하는데, training단계에서 예측된 여러 bounding box 중 실제 객체를 감싸는 ground-truth bounding box와의 IOU가 가장 큰 것 하나의 bounding box를 선택

![Untitled](You%20Only%20Look%20Once%20Unified,%20Real-Time%20Object%20Detec%209961b55244804ec1b0962dfe4e173c8b/Untitled%207.png)

loss function

1_i^obj: grid cell i 안에 객체가 존재하는지 여부(존재하면 1, 존재하지 않으면 0)

1_ij^obj: grid cell i의 j번째 bounding box predictor가 사용되는지 여부

(1) Object가 존재하는 그리드 셀 i의 bounding box predictor j에 대해, x와 y의 loss를 계산

(2) Object가 존재하는 그리드 셀 i의 bounding box predictor j에 대해, w와 h의 loss를 계산

(3) Object가 존재하는 그리드 셀 i의 bounding box predictor j에 대해, confidence score의 loss를 계산(Ci = 1)

(4) Object가 존재하지 않는 그리드 셀 i의 bounding box predictor j에 대해, confidence score의 loss를 계산. (Ci = 0)

(5) Object가 존재하는 그리드 셀 i에 대해, conditional class probability의 loss를 계산. (p_i(c)=1 if class c is correct, otherwise: p_i(c)=0)

λ_coord: coordinates(x, y, w, h)에 대한 loss와 다른 loss들과의 균형을 위한 balancing parameter

λ_noobj: 객체가 있는 box와 없는 box 간에 균형을 위한 balancing parameter

VOC 2007, 2012 훈련 및 검증 데이터 셋을 활용하여 135 epochs로 YOLO 모델을 훈련: batch size=64, momentum=0.9, decay=0.0005로 설정하고, 초반에는 learning rate를 0.001에서 0.01로 천천히 상승시키고 이후 75 epoch 동안에는 0.01, 30 epoch 동안에는 0.001, 그리고 마지막 30 epoch 동안은 0.0001로 learning rate를 설정, overfitting을 막기 위해 dropout=0.5를 적용하고 data augmentation 진행

**2.3. Inference**

파스칼 VOC 데이터 셋에 대해서 YOLO는 한 이미지 당 98개의 bounding box를 예측해주고, 그 bounding box마다 class probabilities을 구함

하지만 YOLO의 grid design에서, 하나의 객체를 여러 그리드 셀이 동시에 검출하는 경우 bounding box가 여러 개 생길 수 있음(multiple detections): non-maximal suppression을 통해 개선

**2.4. Limitations of YOLO**

1. YOLO는 하나의 그리드 셀마다 두 개의 bounding box를 예측 → spatial constraints: 새 떼와 같이 작은 물체가 몰려 있는 경우 공간적 제약 때문에 객체 검출이 제한적임
2. YOLO 모델은 데이터로부터 bounding box를 예측하는 것을 학습하기 때문에 훈련 단계에서 학습하지 못했던 새로운 aspect ratio를 마주하면 힘듬
3. YOLO 모델은 큰 bounding box와 작은 bounding box의 loss가 동일한 가중치 → incorrect localization: 큰 bounding box에 비해 작은 bounding box가 위치 변화에 따른 IOU 변화가 더 심함