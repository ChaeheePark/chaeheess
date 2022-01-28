# Sequence to Sequence Learning with Neural Networks 논문 리뷰

## Abstract

DNN은 학습이 어려운 task에서 훌륭한 성능을 갖는 좋은 모델 -> 그러나 sequence to sequence 에서 사용할 수 없음

**본 논문에서는 sequence 구조 위에서 minimal assumption을 만드는 end-to-end 접근방식을 sequence 학습 방식을 제시 : 고정된 차원의 벡터로 input sequence를 만들기 위해 여러층의 LSTM을 이용, target sequence를 만들어내는 또 다른 깊은 층의 LSTM이 decoder로 동작**

주된 결론은 WMT-14 dataset에 대하여 English to French 기계번역을 하는 것이고 BLEU score 34.8을 획득(STM보다 높은 점수)

LSTM을 STM에서 생성된 1000개 hypotheses로 측정하였을 때는 36.5를 획득

LSTM은 단어의 순서에 민감하게 학습

모든 source sentence에 대해서 단어의 순서를 거꾸로 하였을 때 LSTM의 성능이 향상됨 : 단어의 순서를 거꾸로 함으로 만들어지는 많은 short term dependency는 source와 targe sentence의 최적화 문제를 쉽게 만듬



## Introduction

DNN은 음성인식, 사물인식 등에서 꾸준한 성과를 냄

그러나 input과 target 차원이 고정된다는 한계점 때문에 sequential problem을 제대로 해결할 수 없음

본 논문에서는 일반적인 sequence to sequence 구조를 갖는 task 에 적용가능한 LSTM 구조를 보여줌 

하나의 LSTM이 sequence input 을 사용하여 크고 고정된 차원의 벡터 표현을 얻고 또다른 LSTM은 sequence representive vector로 부터 output을 뽑음 (두번째 LSTM은 RNN이라서 시간에 의존적인 긴 데이터의 학습이 가능하고 입력과 출력사이 시간차를 가지고 있는 application을 해결하는데 도움을 줌)

Neural network에서 sequence to sequence 학습 문제를 해결할 몇가지의 시도들이 있음

우리의 접근은 전체 input sentence를 vector로 매핑하는 연구와 관련됨

<img width="379" alt="image" src="https://user-images.githubusercontent.com/60170358/151308782-59e12533-02e8-4698-a019-45e039337ebe.png">



본 논문의 주된 결과는 WMT14 English to French translation task에서 5개의 LSTM(각 380M parameters)를 이용하여 단순한 left to right beam-search 구조를 통해 34.81 BLEU 점수를 획득- 큰 신경망을 이용한 직역중 가장 좋은 성과

SMT에서 가장 점수가 높은 1000개의 리스트를 가지고 LSTM에서 진행해 본결과 36.5의 BLEU점수 획득

LSTM은 긴 문장에서 어려움을 겪지 않음

-> 긴 source 문장에 대해 단어의 순서를 거꾸로 넣어줬는데 이렇게 함으로써 short term dependencies에 대해서 쉽게 최적화를 진행할 수 있음, SGD로 긴 문장에 대해 어려움없이 LSTM을 학습할 수 있음

LSTM의 유용한 성질은 다양한 길이를 갖고있는 input sentence를 고정된 차원의 vector 표현으로 mapping해줌 

번역은 source sentence의 구문의 경향을 가지고 있다는 측면에서 LSTM을 이용해 그들의 의미를 찾는 문장 표현을 찾는것임, 문장 표현을 찾으면서 유사한 의미는 가까이 두고 아니면 거리를 멀리 떨어트림



## The model

RNN은 input sentence에 대해 output을 아래 수식으로 반복해서 계산함

<img width="146" alt="image" src="https://user-images.githubusercontent.com/60170358/151311136-e6516fac-c010-45fd-8f6f-eaa5245619dd.png">

RNN은 input과 output의 alignment을 미리 알 수 있을 때마다 sequence to sequence를 쉽게 매핑할 수 있음, 그러나 input과 output이 일정한 규칙 없이 다양한 길이를 갖는 문제에 대해서는 적용시킬 수 없음(https://dos-tacos.github.io/concept/alignment-in-rnn/)

그래서 일반적인 sequence를 학습시키는 전략은 **input sequence를 하나의 RNN으로 사용하여 고정된 크기의 벡터에 매핑한 다음, 다른 RNN으로 벡터를 target sequence에 매핑하는 것**

LSTM의 목적은 서로 길이가 다른 input sentence에 대한 output sentence의 조건부 확률을 평가하는것

LSTM은 우선 input sentence에 대하여 고정된 차원의 벡터 v를 계산하고 LSTM-LM의 확률을 계산함

<img width="265" alt="image" src="https://user-images.githubusercontent.com/60170358/151313403-c53fcc59-89a3-4819-a0ab-2ce1e92bd1a3.png">

이 수식에 따라서 각 분포는각 단어에 대한  softmax로 표현되어짐

우리는 Graves에서 나온 LSTM 공식을 사용함 : 각 문장은 <EOS>로 끝남 

우리는 3가지 중요한 방법이 위의 설명과 다름

1. input sequence와 output sequence에 각각 다른 LSTM을 사용함
2. 깊은 LSTM이 얕은 LSTM보다 성능이 훨씬 뛰어나기 때문에 4개의 layer가 있는 LSTM을 선택
3. input 문장의 단어 순서를 반대로 mapping 하는 것이 매우 가치있음을 알음



## Experiments

WMT'14 English to French Machine Translation Task로 2개의 방법을 실험함

SMT의 사용없이 직접적으로 input 문장을 번역 / SMT baseline n-best에 대해 실험

### 1. Dataset details

WMT’14 English to French dataset 사용

348M개의 French word와 304M English word로 구성된 12M의 문장을 이용

source, target language 각각에 fixed size vocabulary를 사용함

### 2. Decoding and Rescoring

본 논문의 실험에서 가장 핵심적인 부분은 크고 깊은 LSTM을 많은 문장쌍에 대해 훈련 시키는 것

<img width="141" alt="image" src="https://user-images.githubusercontent.com/60170358/151316139-2a06f5be-f0b6-487a-97d0-be0a52f09e46.png">

위의 수식을 objective function으로 사용해 log probability를 최대화 시키는 훈련을 진행, 훈련이 끝난 후 아래 수식을 통하여 가장 가능성이 높은 번역을 찾아냄

<img width="105" alt="image" src="https://user-images.githubusercontent.com/60170358/151316391-96733c2a-4bc1-4bdd-83d2-b99ace18dbd3.png">

각각의 timestamp 마다 left to right beam search decoder로 가설들을 확장함

이것은 가설의 수가 매우 많아지기 때문에 log probability에 따라 가장 높은 B개를 제외하고 나머지는 무시함, <EOS> symbol을 만날 때 까지 가설의 크기는 커짐

또한 LSTM을 사용해서 baseline system에 의해 생성된 1000-best를 재검색함

### 3. Reversing the Source Sentences

LSTM은 long term dependencies problem을 해결할 수 있는 능력을 갖고 있지만 source sentence의 단어 순서를 거꾸로 입력하는 것에서 더 좋은 성능을 보일 수 있음 -> dataset에 많은 short term dependencies가 전달한 영향

일반적으로 source sentence와 target sentence가 결합할 때, source sentence의 각 단어는 target sentence와 거리가 먼 단어와 연결되기 때문에 minimal time lag문제가 발생 -> source sentence를 반대로 함으로써 source sentence와 target sentence에 단어 끼리의 평균적인 거리에 변화는 없지만, source sentence의 처음 몇 단어는 target language에 처음 몇 단어와 가깝기 때문에 minimal time lag는 줄어듬 (https://everyday-log.tistory.com/23)

ex) 나는 딸기잼을 샀다 -> 샀다 딸기잼은 나는

따라서 backpropagation은 source sentence와 target sentence 사이에 "establishing communication"이 쉬워지고 결과적으로 성과가 크게 향상됨

### 4. Training details

LSTM model은 쉽게 훈련됨

1000개의 cell, 160000개의 input, 80000개의 output을 갖고 있는 1000차원 word embedding으로 구성된 4개의 layer로 구성된 deep LSTM 사용

### 5. Parallelization

C++로 구현된 deep LSTM 모델은 single GPU를 사용했을때 너무 느려서 GPU 8개 모델을 이용해 병렬화 함, 각 LSTM layer는 서로 다른 GPU에서 실행됨

### 6. Experimental Results

번역의 질을 평가하기 위해 BLEU score을 사용

<img width="459" alt="image" src="https://user-images.githubusercontent.com/60170358/151322528-3abfceff-c75e-4153-9f15-7575eacc5dbc.png">

### 7. Performance on long sentences

<img width="465" alt="image" src="https://user-images.githubusercontent.com/60170358/151323038-d9cae4ec-646a-4967-bf6e-5da3aac8dab1.png">

### 8. Model Analysis

<img width="454" alt="image" src="https://user-images.githubusercontent.com/60170358/151323167-3eec78c7-1035-4266-b709-ee0693ad627f.png">

표현들이 말의 순서에 민감한 반면 능동태를 수동태로 대체하는 것에 상당히 둔감하다는 것을 보임



## Related work

최근에, NNLM에 source lauguage에 대한 정보를 포함하는 방법을 연구하기 시작, 다른 연구는 NNLM을 MT 시스템의 decoder에 통합하고 decoder의 정렬 정보를 사용하여 입력 문장에서 가장 유용한 단어를 NNLM에 제공함 -> 성공적

LSTM-like RNN architecture을 사용해서 문장을 벡터와 back으로 매핑했지만 주로 SMT 시스템을 통합하는데 초점을 맞춘 연구도 있음

end-to-end 훈련도 있음, 하지만 즉시 번역을 생성할 수 없음



## Conclusion

본 연구에서 어휘가 제한된 대규모 심층 LSTM이 , 대규모 MT 작업에서,  어휘가 무제한인 표준 SMT 기반시스템을 능가할 수 있음을 보여줌

source sentence를 거꾸로 해서 얻은 개선의 정도에서도 성과를 이룸

긴 문장을 정확하게 번역하는 LSTM 능력에서도 성과를 이룸