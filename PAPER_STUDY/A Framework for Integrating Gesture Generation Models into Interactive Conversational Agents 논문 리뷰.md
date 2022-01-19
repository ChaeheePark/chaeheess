# Framework for Integrating Gesture Generation Models into Interactive Conversational Agents 논문 리뷰

<img width="343" alt="image" src="https://user-images.githubusercontent.com/60170358/150064901-7d78f9e4-de13-4859-a76c-5c39264fd843.png">

## Abstract

ECAs(Embodied conversational agents) 은 from non-verbal behavior에서 사용자들과 자연스럽고 효과적인 상호작용을 하는데에 장점을 가짐

몸짓은 non-verbal behavior에 가장 필수적인 부분

Gesture generation model은 수년동안 개발됨: rule-based model에서 시작해서 data-driven methods 까지

현재까지 최근의 end-to-end gesture generation methods는 실제 사용자들과 상호작용을 평가해보지 못함

우리는 proof-of-concept framework를 소개- 상호작용에서 현대 gesture generation model의 평가를 용이하게 하기 위해

확장 가능한 open-source framework

1) 3d interactive agent
2) chatbot back-end
3) gesticulating system



## Introduction

ECAs는 적절한 nonverbal behavior일때 더 참여함

존재하는 ECAs는 pre-recorded animations or motion handcrafted specification에 의존, 그러나 최근에 gesture generation 분야의 발전은 data-driven에서 실제적인 gesture을 생성하는것을 가능하게함

실제 사용자들과 상호작용을 평가하지 못했다고 한 이유는 interactive conversational agent를 설정하는 것의 어려움이 있기 때문

**본 연구에서는, data-driven gesture generation model을 conversational agent에 내장하기 위한 framework를 설명 : end-to-end gesticulation model을 사용해서 개발을 가속화하는 framework 구상**

Kucherenko가 개발한 speech and text-driven model과 ECA를 통합하고 두개의 다른 chatbot backends로 접근 방식의 유연성을 보여줌



## System Description

open-source system = Unity의 3d 가상 agent + text-to-speech 기능이 있는 chatbot back-end + 음성에서 gesture 을 생성하는 neural network



### 1. Virtual agent in Unity

가상 환경과 사용자 interface를 Unity 장면으로 제공, 최종 사용자는 voice 또는 text로 conversational agent와 상호작용

application 요구 사항에 따라 character model과 환경을 맞춤화 할 수 있음



### 2. Chatbot backend

사용자의 입력 message는 agent의 응답을 text and audio로 생성하는 chatbot backend로 전송됨 (chatbot backend는 음성 인식 시스템, neural conversational model, text-to-speech 합성기로 구성됨) -> 데모를 위해 이 구성요소의 두가지 구현을 제시

처음으로, google의 DialogFlow platform은 자동음성인식 module과 함께 사용되어 voice-based 상호작용을 agent와 할 수 있게 사용되어짐 (C# 모듈)

두번째 구성에서, free-form conversation 기능들을 가진 virtual agent를 구상하기 위해 open-domain chatbot BlenderBot과 text-to-speech model Glow-TTS를 채택 (Python backend)



### 3. Gesture generation model

chatbot backend의 결과물에 의해, gesture generation model은 대응하는 motion 순서와 합성

Gesticulator라는 최신 gesture generation model을 사용 - 의미의 정보와 결합된 음향 특징을 입력으로 받아들이고 computer animation과 robotics에 널리 사용되는 상체 관절 각도의 sequence로 대응되는 gesture을 생성하는 autoregressive neural network

원본 논문에서는, network는 Trinity Speech-Gesture dataset으로 학습

기본 Gesticulator model을 아래와 같은 조정을 통해 interactive agent에 맞춤 조정함

1) audio feature->spectrogram (합성된 목소리에 더 나은 motion의 결과를 가짐)
2) text 기록은 실정확한 단어 타이밍 정보가 포함되어 잇어 시간 설정에는 사용할 수 없음 -> 사용자와 상호작용 할 때, 음절수에 비례하는 음성 발화 길이로 근사치를 구함
3) BERT word embedding -> FastText로 대체 (특징 추출 시간을 줄이기 위해)



## Limitation

1) 사용가능한 두 chatbot backends는 사용자가 응답하기 전에 agent가 몇 초의 처리 시간을 제공하고 이것은 상호작용에 몰입하는데 영향을 끼침
2) agent의 합성 음성은 분포 외의 audio sample을 야기시켜서 generated motion의 품질을 저하시킴
3) finger motion은 data 품질이 낮기 때문에 gesture generation model에 의해 modeling 되지 않음

하지만, replaceable components는 이러한 한계를 해결할 수 있게 함

예) Gesticulator를 full body motion을 생성하는 모델로 교체하거나 DialogFlow와 Blenderbot을 사용한 두 가지 뚜렷한 예처럼 chatbot backend를 교체



## Conclusions and Future Work

state-of-the art data-driven gesture generation model을 embodied conversational agents와 결합한 framework를 소개했음

앞으로 이 작업이 확장될 수 있는 방향은 많음- Gesticulator와 같은 결정론적 gesture generation model대신 확률론적 gesture generation model을 선택함으로써 다양한 gesture을 얻을 수 있음