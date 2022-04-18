# Emotion Recognition in Conversation: Research Challenges, Datasets, and Recent Advances 논문 리뷰

## Abstrct

Emotion recognition in conversation (ERC) - 대화에서의 감정인식: 사용자의 감정을 이해해야 하는 감정 인식 대화를 생성하는 데에도 매우 중요해서 효과적이고 확장 가능한 대화 감정 인식 알고리즘이 필요



## Introduction

the presence of contextual cues, the temporality in speakers’ turns, or speaker-specific information(맥락적 단서의 존재, 화자의 차례의 시간성 또는 화자 특정 정보)이 ERC에서 중요

<img width="530" alt="image" src="https://user-images.githubusercontent.com/60170358/162694576-05f52e3b-9aa7-413c-9b4a-db0022a4ebd6.png">

(같은 Yeah도 다른 느낌)



### Task Definition

미리 정의된 여러 감정으로부터 각 발화의 감정을 식별하는 것을 목표

<img width="263" alt="image" src="https://user-images.githubusercontent.com/60170358/162696064-853fdd5f-36ad-4731-ba71-966ef95e6218.png">

### Controlling variables in conversations

task-oriented or non-task oriented(채팅) -> 주제, 대화자의 성격, 논증 논리, 관점, 의도에 의해 통제

<img width="272" alt="image" src="https://user-images.githubusercontent.com/60170358/162696567-757e2c26-389c-4776-88e0-96e20691b7d8.png">

**이전 발화**는 과제 지향 대화 또는 화자의 감정 상태(채팅)에서 가장 큰 변화를 주기 때문에 가장 중요한 발화

화자의 상태+화자의 의도+감정+이전발언 = 현재 발언