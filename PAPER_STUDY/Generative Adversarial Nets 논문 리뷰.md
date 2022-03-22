# Generative Adversarial Nets 논문 리뷰

## Abstract

경쟁하는 프로세스를 통해 generative model을 추정하는 새로운 프레임 워크를 제안

생성모델인 G (generative model)는 training data의 분포를 모사 -> discriminative model이 구별하지 못하도록

판별모델인 D (discriminative model)는 sample 데이터가 G로부터 나온 데이터가 아닌 training data로부터 나온 데이터일 확률을 추정

- G를 학습하는 과정은 D가 실수를 할 확률을 최대화하는 것

- minimax two-player game으로 표현

- 임의의 함수 G, D의 공간에서, G가 training 데이터 분포를 모사하게 되면서, D가 1/2임 (즉, 실제 데이터와 G가 생성해내는 데이터의 판별이 어려워짐)

- G와 D가 multi-layer perceptrons으로 정의된 경우, 전체 시스템은 back-propagation을 통해 학습될 수 있음

->  G는 실제 training data의 분포를 모사하며 그와 비슷한 데이터를 생성하려하고, D는 실제 데이터와 G가 생성해낸 데이터를 구별하려하는 경쟁적인 과정



## 1 Introduction

- 지금까지는, 고차원의 방대한 sensory 데이터를 클래스 레이블에 mapping 하는 discriminative 모델 사용-> well-behaved gradient를 갖는 선형 units를 사용한 backpropagation, dropout 알고리즘 기반
-  Deep generative model은 maximum likelihood estimation 관련 전략에서 발생하는 많은 확률론적 계산을 근사화하는 것이 어렵고 generative context에서는, 앞서 모델 사용의 큰 성공을 이끌었던 선형 units의 이점을 활용하는 것이 어렵기 때문에 impact가 덜했음

-> 이 논문에서 소개될 새로운 generative model은 이러한 어려움들을 회피

- 이 논문에서 소개되는 adversarial nets에서, generative model은 상대편, 즉 sample이 모델 분포(G가 생성해낸) 로부터 온 것인지 데이터 분포(실제의) 로부터 온 것인지를 결정하는 것을 배우는 모델
- generative model은 위조지폐를 제작하여 사용하려는 위조지폐범과 유사하다고 생각할 수 있는 반면, discriminative model은 위조지폐를 탐지하려는 경찰과 유사 -> 이러한 경쟁하는 과정의 반복은 어느 순간 위조지폐범이 진짜같은 위조지폐를 만들 수 있고 경찰이 위조지폐를 구별할 수 있는 확률 역시 1/2가 됨

-> 결국 GAN의 핵심 컨셉은 각각의 역할을 가진 두 모델을 통해 적대적 학습을 하면서 가짜와 진짜의 것을 구분 못할때 까지 각 모델의 능력을 키워주는 것

![img](https://firebasestorage.googleapis.com/v0/b/gitbook-x-prod.appspot.com/o/spaces%2F-Lzv9WQqVErrkv4TUmw2%2Fuploads%2FJsIjYOv4MD0d9yya26Iv%2Ffile.png?alt=media)