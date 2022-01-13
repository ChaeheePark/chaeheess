# Language Models are Unsupervised Multitask Learners 논문 리뷰

## Abstract

NLP task는 전형적으로 task에 맞는 dataset으로 학습함-> 본 논문에서는  WebText라는 수백만 웹 페이지의 데이터로 explicit supervision 없이 학습함

language model의 용량은 zero-shot task가 필수적이고 log-linear performance로 성능이 향상

GPT-2는 1.5B의 parameter의 Transformer이고 그것은 8개중 7개의 state를 zero-shot setting을 하여 state of art를 달성함

유망한 자연어 처리 시스템을 구축



## Introduction

ML system의 주된 시도는 desired task에 맞는 올바른 dataset으로 training을 하는것

-> single task training on single datasets의 유행이 현재 system에서 관차되어지는 generalization의 부족에 주요 원인임

Multitask learning은 일반화를 잘하는 유망한 framework, NLP는 초기단계-> 효과적인 많은 훈련쌍이 필요

current techniques에서 dataset을 계속해서 만드는것은 어려움 -> additional setups 을 만들도록 함

<img width="499" alt="image" src="https://user-images.githubusercontent.com/60170358/149265117-cda986ad-f7a8-4d54-aba1-95945780c271.png">

현재 가장 좋은 자연어처리 모델에서의 수행은 pre-trainig과 supervised fine-tuning의 결합

최근에는 task-specific architectures가 필요없이 self-attention blocks로 가능

이 논문에서는 2가지 line을 연결하고 transfer 방법의 일반적인 trend를 이어나감-> LM이 parameter나 architecture 수정을 하지 않고 zero-shot setting으로 downstream 작업을 수행할 수 있음을 보여줌

zero-shot setting에서 광범위한 작업을 수행할 수 있는 Language Model



## Approach

Language model은 비지도 분포 추정-> tractable sampling 이 가능하고 transformer같은 self-attention으로 모델 표현성이 높아짐

single task에서 배우는 것은 조건부 확률 분포를 추정하는 것으로 확률론적 프레임워크로 표현 가능

일반 시스템은 동일한 입력에 대해서도 여러가지 작업을 할 수 있어야 함 -> 수행할 작업에 대해서도 조건을 생성

McCann에서 단일 모델 MAQN을 훈련해서 다양한 형식의 예에서 많은 작업을 유추하고 수행

이 논문에서 추측: 충분한 용량을 가진 LM이 procurement와 상관없이 자연어 처리에서 증명된 작업을 더 잘 예측하고 배울것 -> unsupervised multitask learning -> 다양한 zero-shot seeting과 작업에서 언어모델의 성능을 분석할 것

### Trainig Dataset

크롤러를 제작해서 target dataset에 맞는 document를 포함하게 함

위키피디아와 중복되는 부분을 삭제하고 총 40GB text, 8 milion 개 문서 생성 -> 45 milion 개의 links

### Input Representation

BPE를 사용: character, word level의 중간 지점

byte sequence가 아닌 Unicode points에서 작동함

dog. dog! dog? 같은 많은 변형이 발생하기 때문에 dog와 같은 일반적인 단어들의 많은 버전을 포함

여러개의 단어 토큰에 단어 조각만 최소화 하는 동시에 압축 효율을 크게 향상시키는 공간에 대한 예외를 추가

pre-processing, tokenization, vocab size에 관계없이 모든 데이터 셋에서 우리의 LM을 평가할 수 있게 함

### Model

transformer을 사용 , GPT model에서 약간의 수정한 것

- Layer normalization 을 하위 블록의 입력으로 이동(residual network)
- 마지막 self-attention model 이후의 normalization layer 추가
- 모델 깊이가 있는 residual layers 축적을 설명하는 초기화 사용
- context size 512->1024
- <img width="246" alt="image" src="https://user-images.githubusercontent.com/60170358/149269408-bb27de58-6c00-4da3-872f-f036c961812a.png"> 다른 parameter가 있는 4개의 모델을 학습



## Experiments

4개의 LM 사용, 가장 작은 모델의 크기가 GPT와 동일하고 2번째 작은 모델의 크기가 가장 큰 BERT와 비슷함

가장 큰 모델을 GPT-2라고 부름

<img width="517" alt="image" src="https://user-images.githubusercontent.com/60170358/149328837-29d17870-0989-49d4-94e4-8767fc4fc41f.png">

### Language Modeling

WebText LM의 8개의 dataset 중에서 7개로 zero-show SOTA 만듬

## Children's Book Test

<img width="253" alt="image" src="https://user-images.githubusercontent.com/60170358/149331580-33aed069-0c0d-43ff-a090-fb53db874843.png">

다양한 카테고리의 명사, 동사등을 구분

### LAMBADA

50 token의 마지막 word 예측

### Winograd Schema Challenge

<img width="251" alt="image" src="https://user-images.githubusercontent.com/60170358/149332406-4e4b5c46-839f-4ab8-85f6-3ff660c51e3b.png">

문장을 문맥화 시켜서 모호성 낮춤

### Reading Comprehension

<img width="243" alt="image" src="https://user-images.githubusercontent.com/60170358/149332762-bc953bdb-f7e1-495e-9fa4-3e69e2820a3e.png">

CoQA를 이용해서 QA 테스트 

### Summarization

CNN과 Daily MAil dataset을 이용해서 요약 능력 테스트

### Translation

### Question Answering

<img width="521" alt="image" src="https://user-images.githubusercontent.com/60170358/149333363-98811c35-2402-440a-9e1c-223fa740415e.png">

QA에서 질문의 4.1%을 정확하게 답함

GPT-2는 가장 자신있는 1%의 질문에 63.1% 정확도를 가짐



## Generalization vs Memorization

overlap, similar text가 학습에 끼치는 영향을 알아내는 것은 중요

n-gram overlap을 중요한 검증단계로 사용하고 새로운 NLP datasets에 대한 train and test 분할 동안 sanity check 함



## Related Work

WebText에서 훈련된 LM의 성능을 그래프로 보면 train,test 모두 perplexity가 하락

GPT-2도 underfitting

<img width="256" alt="image" src="https://user-images.githubusercontent.com/60170358/149338836-e4b0c25a-5643-4d00-80fa-7e5185ea5a1b.png">

많은 관련 연구가 있음



## Discussion

unsupervised task learning이 탐구해야할 유망한 연구 영역

->supervised adaption or modification 없이 작업을 직접 수행하는 것을 배움

Supervision 없이 task 를 배우는 pre-training 기술 가능성도 존재, 그러나 초보적인 수준

많은 표준 NLP 작업에 대한 WebText LM의 zero-shot 성능을 연구했지만 사용할 수준은 아님 , 평가할 수 있는 추가 과제들이 많음



## Conclusion

충분히 크고 다양한 dataset에 대해 large language model을 train하면 많은 -domain 및 dataset에서잘 수행 가능

GPT-2 zero-shots 통해 dataset 8개 중 7개 에서 성능 확인

모델이 zero-shot setting 에서 수행할 수 있는 과제의 다양성은 다양한 텍스트 corpus의 가능성을 최대화 하도록 supervision 없이도 작업을 수행할 수 있음을 시사
