# BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding 논문 리뷰



## Introduction

Language model pre-training이 많이 있음

pre-trained 모델을 만드는 전략은 크게 1) feature-based 2) fine-tuning 을 나눌 수 있음 ->  그러나 한계가 존재

대표적인 한계점으로는 unidirectional(단방향) 하다는 것이고 이게 특히 fine-tuning이 기반인 모델을 사용해서 q-a task를 할때 harmful (both direction incoporating X)

-> **따라서, fine-tuning approach의 모델을 향상시키기 위해 BERT 제안**

BERT는 unidirectional의 단점을 완화시키기 위해 MLM(masked launguage model)을 만듬

MLM: input으로 들어온 token들 중 몇몇개의 token을 mask 씨워서 그 mask 씨워진 token이 원래 무슨 token이었는지 예측하는 것 -> 그래서 양방향의 context를 용해시키게 함

NSP(next sentence prediction): text-pair을 사용



## Related Work

1. Unsupervised Feature-based Approaches - ELMO : q-a, sentiment analysis, named entity recognition (task-specific network에 pre-trained language representation을 추가적인 feature로 제공)
2. Unsupervised Fine-tuning Approaches - OpenAI GPT : task-specific한 파라피터를 최소로 하고 모든 pre-trained된 파라미터를 downstream task를 학습을 통해 조금만 바꿔주는 방식
3. Transfer Learning from Supervised Data -ImageNet





## BERT

**BERT**의 아키텍처는 Transformer 구조를 사용하지만, pre-training과 fine-tuning 시의 아키텍처를 조금 다르게하여 transfer learning을 용이하게 만듬

pre-training / fine-tuning의 2가지 step이 있음

pre-training동안 model은 unlabeled된 data를 사용

fine-tuning동안 parameter가 초기화 되고 labeled data를 사용하며 fine-tuning 해가며 parameter 튜닝

BERT Transformer은 bidirectional self-attention을 사용

![image-20211123201057451](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20211123201057451.png)

Input/Output Representations: single sentence / pair of sentence(q-a)

![image-20211123204028223](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20211123204028223.png)

sep : pair 문장 들어갔을 때 나눠줌

cls: 무조건 첫번째 vector

ELMo, GPT는 left-to-right 또는 right-to-left 언어 모델을 사용하여 pre-training을 수행하지만, BERT는 이와 다르게 2가지의 새로운 unsupervised task로 pre-training을 수행

**pre-training BERT**

1) Masked LM : input token 중 랜덤하게 mask하고 이를 학습하여 주변 단어의 context만 보고 masked token을 예측 , 그러나 pre-training과정에서만 사용할 수 있음 (random으로 15% 토큰 선택, 그 토큰 들 중 80- mask, 10-random변경, 10-동일하게둠)
2) NSP: q-a는 두 문장간의 relationship이 있어야함, 그 relationship을 학습 시키기 위해 50%는 a-b 관련있는 문장을 주고 label을 isnext, 나머지는 관련없는 문장을 주고 label은 notnext를 하여 학습



**fine-tuning BERT**

input과 output을 BERT에 연결하고 모든 parameters를 fine-tuning

pre-training에 비해 상대적으로 쉽다





## Experiments

![image-20211123225320460](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20211123225320460.png)

1. GLUE

2. SQaAD: 질문과 문단으로 substring인 정답을 맞춤

3. SWAG

   

## Ablation Studies

1. Effect of Pre-training Tasks

   No NSP: NSP 없앰

   LTR&No NSP: MLM 대신 LTR을 사용하고 NSP를 없앰

- No NSP를 제거하면 성능 저하가 됨
- LTR 모델의 성능은 MLM 모델보다 낮음

2. Effect of Model Size

   모델 사이즈가 커질수록 성능이 좋음

3. Feature-based Approach with BERT

   transformer encoder 구조에 의해 쉽게 표현되어질수 없어서 task-specific model 구조를 필요로 하고, computational benefit의 장점이 있음



## Conclusion

unsupervised pre-training 은 언어모델의 중요한 시스템

bidirectional architecture 발견에 큰 의미

