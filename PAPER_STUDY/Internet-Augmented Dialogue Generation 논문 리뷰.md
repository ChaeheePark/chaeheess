# Blender Bot 2.0: An open source chatbot that builds long-term memory and searches the internet 리뷰

[https://ai.facebook.com/blog/blender-bot-2-an-open-source-chatbot-that-builds-long-term-memory-and-searches-the-internet/](https://ai.facebook.com/blog/blender-bot-2-an-open-source-chatbot-that-builds-long-term-memory-and-searches-the-internet/)

## base paper

- [Internet-Augmented Dialogue Generation](https://parl.ai/projects/sea)

지구상에서 지속적으로 업데이트되는 지식의 가장 큰 저장소는 인터넷 검색을 통해 접근할 수 있다. 이 작업에서는 대화식 에이전트에 인터넷에 대한 액세스를 제공하는 것을 연구-문맥에 따라 인터넷 검색 쿼리를 생성한 다음 검색 결과에 조건을 적용하여 최신 관련 정보를 사용할 수 있는 방법인 응답을 생성하는 방법을 제안

![Untitled](BlenderBot%20535f7/Untitled.png)

- [Beyond Goldfish Memory: Long-Term Open-Domain Conversation](https://parl.ai/projects/msc)

최신 모델은 컨텍스트가 거의 없는 짧은 대화에 대해 훈련되고 평가되고, 장기간의 대화 설정은 거의 연구되지 않았다. 장기 대화 설정에서 어떻게 성능이 떨어지는지를 보여주고 훨씬 더 나은 성능을 발휘할 수 있는 긴 컨텍스트 모델을 연구- 검색 증강 방법과 이전 대화를 요약하고 불러올 수 있는 기능이 현재 최첨단 기술로 간주되는 표준 인코더-디코더 아키텍처를 능가하는 방법을 발견

## 기존 Chatbot (GPT-3 & BlenderBot 1.0)의 문제점

GPT-3와 BlenderBot 1.0은 진행 중인 대화에 대해 명확하게 표현이 가능하고 실제와 같은 대화를 생성할 수 있다. 하지만 GPT-3와 BlenderBot 1.0은 금붕어와 같은 짧은 기억력과 이전에 배운 정적인 지식에 의존한다.

또한 GPT-3와 BenderBot 1.0은 어제 대화했지만 오늘 대화할 때 어제 대화한 내용을 잊어버리고 알고리즘의 결함으로 “hallucinated knowledge(환각 지식)”, 즉 정확하지 않은 지식을 의심없이 이야기한다.

## BlenderBot 2.0의 특징

BlenderBot 2.0은 인터넷 검색 쿼리를 생성할 수 있고 시간이 지나감에 따라 지식을 추가하면서 이전 아이디어를 참조할 수 있는 능력을 가진 최초의 챗봇이다.

- Long-term Memory : BlenderBot 2.0은 Long-term Memory 모듈을 구현한 덕분에 사용자와 며칠, 몇 주, 몇 달 동안 지속된 여러 대화들의 토픽들을 기억할 수 있다.
- Personalization : 대화 히스토리는 개인별로 분리되어 저장되며 한 사용자와 대화로부터 배운 새로운 정보를 다른 사용자의 대화에 사용되지 않도록 분리된다.
- Internet Search: 대화 중에 BlenderBot 2.0은 상황에 맞는 인터넷 검색 쿼리를 생성한 후, 그 검색 결과를 사용자의 질문에 대한 응답에 통합한다. 이렇게 하여 최신 정보를 유지할 수 있다.

## BlenderBot2.0의 모델 아키텍처

- 사용자와 대화 히스토리는 BlenderBot 2.0의 Encoder와 Memory Decoder를 거쳐 Long-term memory에 저장된다.
- 사용자와 대화 중에 BlenerBot 2.0은 Query generator를 통해 검색 쿼리를 생성한 후 인터넷 검색 및 Long-term 메모리로부터 관련 정보를 찾는다.
- 인터넷 검색 결과와 Long-term 메모리로부터 찾은 결과는 Encoder로 입력되고 Encoder의 출력들은 하나로 합친(Concatenate) 후에 Decoder로 입력된다. Decoder는 최종 응답을 출력한다.

![Untitled](BlenderBot%20535f7/Untitled%201.png)

## 테스트 결과

BlenderBot 1.0은 이미 Meena와 DialoGPT와 같은 다른 챗봇을 압도하는 성능을 보여줬다. 새로운 Blenderbot 2.0은 BlenderBot 1.0을 능가하고 있다.

1. 참여 평가(이전 커뮤니케이션 세션이 끝난 곳에서 계속 커뮤니케이션하는 능력) : 17% 향상
2. 이전 커뮤니케이션 세션의 맥락을 고려한 평가: 55% 향상
3. 환각을 9.1% → 3.0%로 줄임

## 시사점

기존 Chatbot는 기억하지 않아 개인화가 어려웠던 문제가 있었다. BlenderBot 2.0은 Long-term 메모리를 추가하여 Chatbot에 인간이 가지고 있는 기억을 부여하여 개인적인 대화가 가능하여 인간과 닮은 대화가 가능해졌다.
Long-term 메모리는 개인화를 위해 중요한 모듈으로 Chatbot의 대화 히스토리가 축적하려면 모델 weight 외에 사용자의 히스토리를 저장할 수 있는 추가적인 메모리 공간이 필요할 것으로 생각된다.
학습된 지식 외에 인터넷 검색을 통해 좀더 정확한 응답을 선택할 수 있게 되었었다. 이것은 모든 지식을 학습에 의존하지 않고 필요할 때마다 인터넷 검색을 통해 보충할 수있어 새로운 지식을 보충하기 위해 Large 모델을 주기적으로 학습해야 하는 어려움이 크게 감소할 수 있을 것으로 예상된다.