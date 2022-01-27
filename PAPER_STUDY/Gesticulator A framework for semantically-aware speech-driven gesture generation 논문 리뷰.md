# Gesticulator: A framework for semantically-aware speech-driven gesture generation 논문 리뷰

## Abstract

말을 하는 동안 사람들은 손짓을 함 -> 정보를 전달하는데 중요한 역할

현재의 end-to-end co-speech gesture generation system은 음성을 표현하기 위해 오디오 또는 텍스트를 사용 -> 음향적으로 연계된 제스처나 의미론적으로 연계된 제스처를 생성하는것으로 제한

**임의의 beat와 의미있는 제스처를 함께 생성하도록 설계된 모델을 제시**

음성의 음향 및 의미적 표현을 input으로 사용하고 관절 각도 회전의 연속을 output으로 생성



## Introduction

사람들은 말을 하면서 손짓을 함 : co-speech gesture

가상 agent 상호작용은 언어+비언어적 행동을 동반할 때 더 관여, 제스처 특성을 조작하면 agent의 감정에 대한 사용자의 인식에 영향을 미침

전통적으로, 가상 agent에 대한 gesture 생성은 다양한 규칙 기반 시스템에서 수행됨-> 이 접근법은 agent가 만들어내는 gesture의 이산적인 set에 의해 제한됨

최근에는 딥러닝의 발전으로 data-driven 접근법이 대두됨 (지정된 제스처로 분류-> 연속적인 모션 생성하는 회귀)

**연속적인 제스처 생성 : 이전 연구에서는 음성 또는 의미론적의 단일 입력 양식을 사용 -> 두가지 양식을 모두 사용**

1) 음성과 의미 특징을 연속적인 3d 제스처로 매핑하는 data-driven model
2) 다른 아키텍처와 중요한 모델링 선택을 비교
3) 결과 제스처에 대한 두가지 음성 양식(음성과 의미)의 영향에 대한 객관적,주관적 평가

수동 텍스트를 통해 3d 공식 음성 제스처 말뭉치인 Trinity College dataset을 확장해서 사용



## Background And Related Work

### 1. Background

음성과 제스처는 강하게 연관됨

gesture classification(제스처와 말 사이의 시간적인 정렬 뿐만 아니라 제스처 생성 문제 공식)

1. Co-speech Gesture Type
2. Gesture-Speech Alignment (여러 언어 사용)
3. Gesture-Generation

### 2. Related Work

1. Audio-Driven Gesture Generation
2. Text-Transcription-Driven Gesture Generation
3. Multimodal Gesture-Generation Models
4. Regarding Motion Continuity



## Training And Test Data

기계학습을 사용해서 제스처 생성 모델을 개발

Trinity Gesture Datset 사용 (다양한 주제에 대해 자유롭게 말하는 남성 배우의 244분의 음성과 모션 녹화)

original 69개 중 15개의 상체 관절을 남기고 하반신 데이터를 제거

### 1. Test-Segment Selection

dataset에서 10분짜리 2개의 녹화가 훈련에서 나옴

test를 위해 10초 의 50개의 segment 선택 (30개의 무작위 segment와 20개의 의미론적 segment)

세명의 주석자들은 기록된  gesture가 음성 내용과 의미론적으로 기록된 내용이 연결되는 시간 간격을 표시함 (5초 범위 이내는 사용)

### 2. Audio-Text Alignment

<img width="263" alt="image" src="https://user-images.githubusercontent.com/60170358/151180731-124ac77d-347e-4e9d-bc41-33353367eaa3.png">

텍스트와 음성은 일반적으로 sequence 길이가 달라서 위의 그림과 같이 단어를 frame 수준의 특징으로 인코딩함 

filler 단어를 제외한 문장은 BERT에 의해 인코딩 됨

filler 단어와 silence는 Vf와 Vs로 인코딩 

filler 단어는 사고 과정을 나타내며 다양한 제스처에 의해 발생 

silence는 전형적으로 gesticulation이 없어서 모든 요소를 -15로 설정해 다른 인코딩과 구별되게 함

ASR시스템의 시간을 사용해서 텍스트와 음성 특징 sequence의 길이와 시간이 동일하도록 균일하게 upsampling함 -> 음성 합성 분야의 표준 text-speech 정렬 방법



## Speech-driven Gesture Generation

음성과 의미에서 상체를 생성하는 방법

### 1. Feature Types

제스처 생성의 이전 연구와 마찬가지로 log-power mel-spectrogram 특징으로 음성 오디오를 표현-> 64차원 음향 특징 벡터를 추출함

의미론적 특징을 위해 영어 위키백과에서 사전학습된 BERT를 사용

각 문장은 BERT에 의해 인코딩 되어 오디오에 맞춰 단어 당 768개의 특징 생성

<img width="248" alt="image" src="https://user-images.githubusercontent.com/60170358/151190894-b872b7ac-b611-4c5e-b2ab-ad10bb3d282a.png">

위의 프레임별 스칼라 기능으로 보완

모션특징을 추출하기 위해 모션 캡처 데이터를 20fps로 downsampling하고 관절 각도를 T-pose에 상대적인 지수 map표로 변환

그 후 PCA를 적용하고 train data 분산을 95%로 유지하으로써 차원성을 줄임

-> 12개의 구성요소가 생성

### 2. Model Architecture and Training

간단한 모델을 개발하는것

<img width="243" alt="image" src="https://user-images.githubusercontent.com/60170358/151200295-50509559-fde4-4a73-926d-bca5828c7b85.png">

각 프레임의 텍스트 및 오디오 기능은 차원성을 줄이기 위해 feed-forward 신경망에 의해 공동으로 인코딩됨

현재 프레임을 예측하기 위한 많은 입력 문맥을 제공하기 위해 과거 음성의 0.5초와 미래 음성의 1초의 창을 통과시킴

문맥 창 내부의 인코딩은 긴 벡터로 연결되고 fully-connected layer을 통과함

autoregressive함 : 이전 모델 예측을 모션 연속성을 보장하기 위해 모델에 다시 공급함

이전 포즈의 정보를 알기 위해, 일반적 연결을 하는 FiLM 조건화 사용 (이전 포즈를 입력으로 취하는 신경망에 의해 affine 변환을 적용)

### 3. Training Procedure

dataset에서 정렬된 음성, 텍스트, 제스처 sequence에 대해 훈련

각 훈련 sequence는 연속 프레임 70개가 포함됨

모델은 SGD와 Adam을 사용하고 end-to-end로 최적화됨

개발 중, 이전 포즈의 정보가 음성 정보를 압도하는 경향이 있음을 관찰 (초기 모델은 음성 입력을 독립적으로 수행해서 정적포즈가 나왔음)

-> 이것은 흔한일, 그래서 대응하기 위해 모델이 autoregressive 입력을 받기 전 처음 7 epoch 동안 autoregressive 를 점진적으로 훈련함 , 이것이 음성 입력에서 유용한 특징을 추출하는 방법을 배우는 데 도움이 됨

### 4. Hyper-Parameter Settings

Tune 도구를 사용해서 hyper-parameter을 검색

활성화 함수는 tanh, batch size는 64, learning rate는 10^-4, dropout은 0.2



## Evaluation Measures

### 1. Objective Measures

RMSE, 가속과 jerk(가속도 변화율), 가속도 히스토그램을 평가 -> 관절 각도에서 3d 관절 위치로 변환

가속과 jerk로 모든 프레임에서 평균을 산출, 움직임 통계를 더 자세히 조사하기 위해 움직임의 속도 히스토그램을 계산하고 비교함

### 2. Subjective Measures

user study 진행

1. Experiment Design : 사람 모집해서 집중력 테스트
2. Experimental Procedure (?)



## Ablation Study

<img width="228" alt="image" src="https://user-images.githubusercontent.com/60170358/151284306-87e10ac4-d56e-4ffb-a21a-ed7032a7b311.png">

### 1. Objective Evaluation

1. Average Motion Statistics

<img width="249" alt="image" src="https://user-images.githubusercontent.com/60170358/151284378-23397778-0864-4107-bd25-5b2957cb9ad9.png">

2. Motion Velocity Histograms

<img width="251" alt="image" src="https://user-images.githubusercontent.com/60170358/151285428-595cc016-b313-4842-a258-72757888c37b.png">

### 2. First Perceptual Study

<img width="536" alt="image" src="https://user-images.githubusercontent.com/60170358/151287483-92a5c225-4f38-4f4d-9a7d-ecac33064eb9.png">

-> No PCA 가 사람들이 가장 선호함

### 3. Relation between objective and subjective evaluations

"No FiLM"과 "No Velocity Loss" 조건 에서, 사소한 변경만 발생시킨 ablation의 경우 사용자 평가가 크게 변경되지는 않음

원본 데이터에 비해 낮은 jerk의 모델의 경우에 사용자들은 "No PCA"를 선호함

그러나 너무 높은 jerk는 "No Autoregression" 과 관련이 있음

-> jerk 분석이 인간같은 운동에 대한 정보를 제공해줌



## Additional Evaluations and Comparisions

**이 연구의 주요 목표는 의미와 음성을 모두 고려한 연속적인 제스처 생성을 위한 첫번째 모델을 개발하는 것**

Ginosar이 제안한 CNN-GAN 모델과 비교

### 1. Comparing with the state-of-the-art

<img width="214" alt="image" src="https://user-images.githubusercontent.com/60170358/151289530-35fa2bf0-64c8-4764-bc5e-ee41fcd5fa98.png">

위의 표는 50개의 test sequencce에 대한 평균 가속 및 jerk 를 나타냄

제안된 방법이 원래 데이터 값보다 절반을 갖는 반면 CNN-GAN은 대신 2배를 갖음

<img width="255" alt="image" src="https://user-images.githubusercontent.com/60170358/151289719-62863796-0ed4-4f75-a305-f68e3d92cfd8.png">

사용자 연구를 통해 CNN-GAN 보다 우리의 모델이 더 선호되었음을 확인함

### 2. Comparision with the Ground Truth

사용자 연구에서 모든 질문에서 실제 데이터 움직임에 대한 상당한 선호도가 있었음

### 3.  What Do “Semantic” Gestures Even Mean?

text 입력이 모델이 상징적, 은유적, 직설적인 의미론적 제스처를 더 많이 생성하는데 도움이 되는지 평가함

텍스트 정보가 있고 없는 2가지 최상의 모델을 비교함 -> 사용자 연구에서 모두 텍스트가 없는 모델보다 텍스트를 사용한 모델의 음성 콘텐츠와 제스처가 연계되어있다고 많이 표시했지만, 동의도가 낮음 : 어떤 세그먼트가 의미적이었는지에 대한 낮은 동의는 어떤 제스처가 의미적으로 연결된것으로 분류되어야 하는지가 매우 주관적인것을 나타냄



## Conclusion and Future Work

1. 오디오와 텍스트를 모두 사용하면 연속적인 제스처 생성 모델을 개선할 수 있음
2. autoregressive는 현대의 제스처 생성 모델에서는 흔하지 않지만 vanishig gradient 문제 없이 제스처의 연속성을 나타낼 수 있음
3. PCA는 제스처 생성범위를 줄임
4. 연구 참가자들은 우리모델을 더 선호함



이 연구의 한계는 텍스트가 포함된 주석달린 dataset이 필요하다는 것 -> 자동 음성 인식에 모델을 직접 교육하는 것을 고려할 수 있음

미래 연구에서는 모델 확률적을 만들고 더 큰 dataset을 사용하고 다른 제스처 유형을 별도로 처리함으로써 제스처의 의미 일관성을 향상시키는 것을 포함함

