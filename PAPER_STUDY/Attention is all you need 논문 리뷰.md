# Attention is all you need 논문 리뷰

## Abstract

recurrent encoder-decoder model에 기반한 attention mechanism 이 가장 좋은 성능을 냈었음 

-> 그래서 새로운 simple network architecture인 Transformer을 제안 (recurrence와 convolution을 완전히 생략한 attention을 이용)



## Introduction

Recurrent neural network (RNN)

- input과 output의 symbol position으로 계산
- 이전 hidden state로 현재 hidden state 생성
- sequential한 계산에 문제점이 존재

Attention mechanism

- input과 output의 거리에 의존하지 않고 모델링

Transformer

- attention mechaninsm에 전적으로 의존해서 입력과 출력의 global dependence draw ?



## Background

sequential한 계산 줄이기 

- Extended Neural GPU, ByteNet, ConvS2S
- 먼 position 사이의 의존을 어렵게 만듬
- Transformer는, 이를 상수 시간의 계산으로 진행

Self-attention

- different position의 sequence들을 계산하기 위한 single sequence과 관련 
- reading comprehension, abstractive summarization, textual entialment, learning task-independent sentence representation에 쓰임

End-to-end memory 

- recurrent attention에 기반
- question answering, launguage model task에 쓰임



## Model Architecture

encoder-decoder의 단위가 N개로 구성되는 구조

### 1) Encoder and Decoder Stacks

<img width="307" alt="image" src="https://user-images.githubusercontent.com/60170358/148228588-fa501a0e-0922-4601-acce-2137fe30bb34.png">

