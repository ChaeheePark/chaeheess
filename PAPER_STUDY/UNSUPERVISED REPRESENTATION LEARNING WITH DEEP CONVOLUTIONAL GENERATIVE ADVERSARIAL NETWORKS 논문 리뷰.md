# UNSUPERVISED REPRESENTATION LEARNING WITH DEEP CONVOLUTIONAL GENERATIVE ADVERSARIAL NETWORKS 논문 리뷰

## Abstract

최근 몇년 동안 CNN의 비지도학습 보다 지도학습이 더 주목을 받음

본 논문에서는 DCGAN이라고 하는 CNN의 비지도학습을 입증

## 1. Introduction

레이블이 지정되지 않은 대규모 dataset에서 재사용가능한 feature 표현을 학습하는 것은 활발한 연구 영역

컴퓨터비전의 관점에서, unlabeled의 무한한 양의 이미지와 비디오를 활용해서 좋은 중간층 표현을 배울수 있으며 이미지 분류와 같은 다양한 지도학습 과제에 사용될 수 있음

좋은 이미지 표현을 만드는 한가지 방법은 GAN을 훈련시키고 나중에 generator와 discriminator의 네트워크의 일부를 지도학습의 feature 추출기로 재사용하는 것임

또한 학습 과정과 경험적 비용 함수의 부재가 representation learning 에 매력적임

GAN은 훈련하기에 불안정하고 다층 GAN의 중간층 표현을 이해하고 시각화하는 연구는 제한적임

본연구에서 제안하는 것

- convolutional GAN의 architecture 분류에 대한 일련의 제약 조건을 제안하고 평가함 : DCGAN(Deep Convolutional GAN)
- 이미지 분류 작업에 훈련된 discriminator을 사용
- GAN에 의해 학습된 filter을 시각화하고 특정 filter가 특정 객체를 그리는 방법을 배웠음을 경험적으로 보여줌
- generator가 생성딘 샘플의 많은 의미적 특정을 쉽게 조작할 수 있는 흥미로운 벡터 산술 속성을 갖고 있음



## 2. Related Work

1) REPRESENTATION LEARNING FROM UNLABELED DATA

   비지도 표현 학습의 일반적인 접근 방식은 데이터를 군집화 하는것

   이미지의 맥락에서 이미지 patches의 계층적 클러스터링을 수행하여서 강력한 이미지 표현을 배울 수 있음

   또 다른 방법은 auto encoder을 학습해서 이미지를 compact code로 인코딩하는 코드, 사다리 구조의 구성 요소와 위치를 구분하고 코드를 디코딩해서 이미지를 최대한 정확하게 재구성하는것

   -> 이미지 픽셀에서 우수한 feature 표현을 학습함

2. GENERATING NATURAL IMAGES

   생성 이미지 모델은 잘 연구되어 있으며 parametric과 non-parametric 두가지로 나눠짐

   - non-parametric 모델은 기존 이미지 database와 일치하는 이미지 patches에서 일치를 수행하며 텍스처 합성, 초해상도, in-painting에서 사용됨
   - 이미지 생성을 위한 parametric 모델은 광범위하게 탐구되어 옴, 그러나 현실세계의 자연스러운 이미지를 생성하는 것은 성공을 거두지 못함 -> iterative forward diffusion process로 이미지를 생성하는 방법 (https://developers-shack.tistory.com/8)
   - GAN은 노이즈가 많고 이해할 수 없는 이미지를 생성함, 그러나 라플라시안이나 recurrent network , deconvolution network 등은 자연적인 이미지 생성에 성공하였지만 지도학습에 generator을 활용하지 않음

3.  VISUALIZING THE INTERNALS OF CNNS

   신경망을 사용하는 한가지 지속적인 비판은 블랙박스 방식이라는 것

   CNN 의 맥락에서 deconvolution을 사용하고 최대 activation을 filtering 함으로써 네트워크에서 각 convolution filter의 대략적인 목적을 찾을 수 있다는 것을 보여줌

   또한 입력에 gradient descent 를 사용하면 필터의 subset을 활성화하는 이상적인 이미지를 조사할 수 있음

## 3. APPROACH AND MODEL ARCHITECTURE

CNN을 사용해서 이미지를 모델링하는 GAN을 확장하려는 과거의 시도는 성공하지 못함-> LAPGAN의 저자들은 반복적으로 upscale 저해상도 생성 이미지에 대한 대체 접근법을 개발

CNN 아키텍처를 사용해서 GAN을 확장하려는 시도에도 어려움을 겪음 -> 광범위한 모델 탐색 끝에 다양한 데이터셋에 걸쳐 안정적인 교육을 제공하고 더 높은 해상도와 더 깊은 generator model을 학습할 수 있는 architecture을 찾음 : 최근 CNN architecture의 3가지 변경사항을 채택하고 수정함

1) 결정론적 spatial pooling function을 경사형 convolution으로 대체해서 네트워크가 자체 공간을 downsampling을 학습 할 수 있도록 하는 전체 컨볼루션 네트워크
2) 컨볼루션 features 위에 fully connected layer을 제거함
3) Batch Normalization을 각 단위에 대한 입력값을 0으로 정규화

Architecture guidelines for stable Deep Convolutional GANs

- generator/discriminator 에 배치정규화 사용
- fully connected layer 제거
- generator 활성화함수 - 출력층 : Tanh, 나머지 : ReLU
- discriminator 활성화함수 - LeakyReLU



## 4. DETAILS OF ADVERSARIAL TRAINING 

<img width="362" alt="image" src="https://user-images.githubusercontent.com/60170358/158720874-cda640c0-51a9-4ea9-960d-66241e811614.png">



### 1) LSUN

LSUN bedrooms dataset으로 학습

generator가 훈련 이미지를 기억해버릴 가능성을 낮추기 위해 서로 매우 비슷한 near duplicates 인 이미지를 제거

**유사중복이미지 제거과정에서 de-noising autoencoder를 사용**하였는데, 원본 이미지들을 오토인코더의 인코더에 input하여 coding 벡터로 인코딩하고 이 코딩값들이 서로 비슷한 관측들을 설정한 비율에 맞는 선까지 제거되도록 했다고 함.

<img width="361" alt="image" src="https://user-images.githubusercontent.com/60170358/158721573-e5a6806c-9a98-421b-a454-7bed22aa4998.png">

5 epochs  training



### 2) FACES

1만명의 사람에 대해 3백만장의 이미지

OpenCV의 face detector를 돌려서 얼굴에 bounding box를 한 이미지들을 이용



### 3) ImageNet 1-k

비지도학습을 위한 자연이미지 소스로 활용



## 5. EMPIRICAL VALIDATION OF DCGANS CAPABILITIES

### 1) Classifying CIFAR-10 using GANs as a feature extractor

DCGAN에 의해 학습된 representation의 품질을 평가하기 위해 이미지넷1k 데이터셋에 pretrain 시킨 뒤 discriminator의 모든 레이어들의 convolution feature들을 적절한 사이즈가 되게끔 pooling하고 concat하여 벡터를 만든뒤 SVM을 붙여 CIFAR-10 데이터셋에 대해 분류 TASK를 수행

<img width="346" alt="image" src="https://user-images.githubusercontent.com/60170358/158736407-9288737e-69f5-4c37-b656-52183025b895.png">

