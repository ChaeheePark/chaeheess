# Autoencoders 논문 리뷰

## Abstract

**autoencoder는 입력을 압축되고 의미 있는 표현으로 encoding 한 다음, 재구성된 입력이 가능한 원래 입력과 비슷하도록 다시 decoding 하도록 설계된 신경망**



## 1 Autoencoders

input을 reconstruct을 하도록 학습시키는 neural network

-> clustering같은 다양한 의미에 사용할 수 있는 데이터의 "informative" 표현을 unsupervised 방식으로 학습

<img width="262" alt="image" src="https://user-images.githubusercontent.com/60170358/172117756-63ba7902-ece3-4cee-af62-709cf11db790.png">A-> 

A: input x를 encoder에 통과시킨 값 <img width="85" alt="image" src="https://user-images.githubusercontent.com/60170358/172118129-545f1bcb-36ba-411a-9e32-11c958f7d9ce.png">

B: encoder에 통과시킨 값을 decoder에 통과시킨 값 <img width="83" alt="image" src="https://user-images.githubusercontent.com/60170358/172118170-65aacefc-c4d0-44d7-bc07-dc15eaad35b8.png">

output of the decoder 과 input x 사이의 loss를 최소화 하는 방식으로 학습됨


<img width="355" alt="image" src="https://user-images.githubusercontent.com/60170358/172118313-4bbf90ab-24b0-46c4-b403-40efb8535f98.png">

autoencoder는 end-to-end 또는 layer-by-layer로 학습됨



## 2 Regularized autoencoders

training 동안, A와 B에 대한 identity연산자를 얻을 수 있기 때문에, achieved representation을 입력과 동일하게 유지하려면 추가적인 정규화가 필요

가장 흔한 방법은 representation의 차원을 input보다 작게 만드는 방법 (병목현상이 생김)

hidden layer의 크기가 input의 크기보다 크거나 같은 경우 encoder가 단순히 identity 함수를 학습할 위험이 있음-> 병목 현상을 만들지 않는 정규화를 위한 몇 가지 옵션이 존재

autoencoder에서 bias-variance tradeoff가 중요-> autoencoder의 아키텍처가 input을 잘 reconstruct할 수 있게 원하지만, low representation이 의미있는 것으로 일반화되기를 원함



### 2.1 Sparse Autoencoders

tradeoff를 처리하는 한가지 방법은 hidden activation에 희소성을 적용하는 것-> 이 기능은 병목 현상 적용 위에 추가하거나 병목 현상 대신 추가

1) **L1 regularization 적용**

<img width="373" alt="image" src="https://user-images.githubusercontent.com/60170358/172127096-b9e12875-ec34-4f63-806e-b3d21c9b661c.png">

​	autoencoder의 objective은 식(2)와 같게 됨

​	<img width="27" alt="image" src="https://user-images.githubusercontent.com/60170358/172127303-09e1d247-0e83-4beb-a37c-e4458284203e.png">는 hidden layer의 i번째 activation

2. **KL-divergence**

   두 확률 분포 사이의 거리를 측정한 것-> L1 정규화에서 lambda를 조정하는 대신, 각 neuron의 activation이 확률 α를 갖는 베르누이 변수로 작용한다고 가정하고 해당 확률을 조정하여 각 batch에서 실제 확률이 측정되고 차이가 계산되어 regularization factor로 적용

   <img width="311" alt="image" src="https://user-images.githubusercontent.com/60170358/172128354-f729f39e-1c0d-4c25-8ae6-10c9d07749fc.png">

   각 neuron j에 계산된 경험적 확률은 <img width="118" alt="image" src="https://user-images.githubusercontent.com/60170358/172128520-81685338-ff8b-4482-a23e-057e55705fdc.png">

   p와 <img width="22" alt="image" src="https://user-images.githubusercontent.com/60170358/172129371-47b2be7d-22f2-4eeb-956d-1e441f37da70.png">을 같게하는것이 regularization term의 목적



### 2.2 Denoising Autoencoders

<img width="346" alt="image" src="https://user-images.githubusercontent.com/60170358/172129494-36eed890-33aa-4fd5-83d1-de3204a49bdd.png">

Denoising Autoencoder은 regularization option 또는 error correction으로 사용될 수 있는 강력한 autoencoder-> input은 일부 노이즈(white Gaussian noise 등)가 추가됨

<img width="31" alt="image" src="https://user-images.githubusercontent.com/60170358/172153770-bb2d073a-f768-4880-87a1-2285ce0bfcb0.png"> 는 random 변수이고, <img width="36" alt="image" src="https://user-images.githubusercontent.com/60170358/172153829-2b453438-a07b-416a-8f51-ba14343a338c.png">의 분포를 따름



<img width="353" alt="image" src="https://user-images.githubusercontent.com/60170358/172153167-8f73f263-709e-497f-b424-05ba4eca9225.png">

식 (4)는 <img width="25" alt="image" src="https://user-images.githubusercontent.com/60170358/172153281-47c64966-6891-4ef4-a534-caa0e216d110.png">가 noise의 정도를 결정

식 (5)는 p가 x의 값이 무효화되지 않을 확률을 설정



### 2.3 Contractive Autoencoders

Contractive autoencoders에서, decoder에 의한 reconstruction에 중요하지 않은 input의 changes를 무시하도록 해서 feature extraction이 작은 변화에 덜 민감하게 만드는 데 중점

따라서 네트워크의 Jacobian에 대해 penalty가 부과

Jacobian matrix의 hidden layer h는 입력 x의 각 값 xi에 대한 각 노드 hj의 미분들로 구성됨-><img width="107" alt="image" src="https://user-images.githubusercontent.com/60170358/172155386-108c84c8-94ab-4362-8652-cc2e9b334dbb.png">

<img width="337" alt="image" src="https://user-images.githubusercontent.com/60170358/172155310-f6099d32-9632-444c-bb03-52ad832d050a.png">

제곱 Jacobian norm을 최소화함으로써 input의 모든 latent representation은 서로 더 유사한 경향이 있으며, representation 간의 차이가 더 작기 때문에 reconstruction을 더 어렵게 만듬

reconstruction에 중요하지 않은 latent representation의 variation은 regularization 요인에 의해 감소하는 반면, 중요한 variation은 reconstruction error에 대한 영향 때문에 남아 있을 것



## 3 Variational Autoencoders

autoencoder의 표현 능력이 크게 향상된 것은 Variational Autoencoders(VAE) 모델 때문-> Variational Bayes (VB) Inference에 따르면, VAE는 확률적 분포를 통한 데이터 생성을 설명하는 generative model(확률론적 decoder와 동일)

<img width="301" alt="image" src="https://user-images.githubusercontent.com/60170358/172160854-ce1151e2-760c-4089-8a43-b16a71cece2d.png"> 

관찰되지 않은 random latent variable zi에 대해 조건화된 각 데이터 xi에 대한 generative model을 가정하며,  𝜃는 generative 분포를 지배하는 parameter

대칭적으로, 우리는 recognition 표시된 기준 xi가 주어진 latent variable zi에 대한 대략적인 사후 분포를 가정하며, 이는 확률론적 encoder에 해당하며 매개 변수  𝜙에 의해 제어됨

마지막으로, <img width="42" alt="image" src="https://user-images.githubusercontent.com/60170358/172166110-a42fe869-bff0-4fee-923d-452c8f735480.png">로 표시된 잠재 변수 zi에 대한 사전 분포를 가정

매개변수 𝜃와 𝜙는 알 수 없으며 데이터에서 학습해야 함

관찰된 latent variable zi는 recognition model <img width="65" alt="image" src="https://user-images.githubusercontent.com/60170358/172162096-86f900a1-0d3a-4814-82ea-be0734045588.png">에 의해 주어진 코드로 해석될 수 있음



