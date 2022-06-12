# Autoencoders(Dor Bank, Noam Koenigstein, Raja Giryes) 논문 리뷰

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

autoencoder의 표현 능력이 크게 향상된 것은 Variational Autoencoders(VAE) 모델 때문-> Variational Bayes (VB) Inference에 따르면, VAE는 확률적 분포를 통한 데이터 생성을 설명하는 generative model (확률론적 decoder와 동일)

<img width="301" alt="image" src="https://user-images.githubusercontent.com/60170358/172160854-ce1151e2-760c-4089-8a43-b16a71cece2d.png"> 

각 data <img width="20" alt="image" src="https://user-images.githubusercontent.com/60170358/173175917-75d844ea-c600-422e-b4f2-d9b0f9f57364.png">는 관찰되지 않은 random latent variables <img width="19" alt="image" src="https://user-images.githubusercontent.com/60170358/173175931-545346f2-af45-440a-8bc8-0e63fe0c679c.png">로 조건화되어있다고 가정하고  𝜃는 generative 분포를 지배하는 parameter <-> 대칭적으로, 관찰된 <img width="20" alt="image" src="https://user-images.githubusercontent.com/60170358/173175917-75d844ea-c600-422e-b4f2-d9b0f9f57364.png">가 주어진 latent variable <img width="19" alt="image" src="https://user-images.githubusercontent.com/60170358/173175931-545346f2-af45-440a-8bc8-0e63fe0c679c.png">에 대한 대략적인 사후분포를 가정하고  𝜙는 확률론적 encoder을 지배하는 parameter

마지막으로, <img width="42" alt="image" src="https://user-images.githubusercontent.com/60170358/172166110-a42fe869-bff0-4fee-923d-452c8f735480.png">로 표시된 latent variables <img width="19" alt="image" src="https://user-images.githubusercontent.com/60170358/173175931-545346f2-af45-440a-8bc8-0e63fe0c679c.png">대한 사전 분포를 가정

매개변수 𝜃와 𝜙는 알 수 없으며 데이터에서 학습해야 함



log-likelihood 는 각각 data points의 합 <img width="209" alt="image" src="https://user-images.githubusercontent.com/60170358/173175709-42a98872-7a11-4103-93a8-fe20d7fec942.png"> 으로 표현되고 각각 data points들은 식 (7)과 같이 나타냄

<img width="367" alt="image" src="https://user-images.githubusercontent.com/60170358/173175534-2ed98baa-afa0-4dfe-82fb-c1210a0d9231.png">

첫번째 항은 KL divergence이고, 두번째 항은 variational lower bound on the marginal likelihood(<img width="314" alt="image" src="https://user-images.githubusercontent.com/60170358/173177316-74b8a0b4-4031-4050-b8b2-ff1e078cb700.png">) 

-> KL divergence는 음수가 아니기 때문에 lower bound를 최대화 하는 쪽으로 해야 사후분포의 approximation이 향상(매개변수 𝜃와 𝜙들을 조정하면서)



### 3.1 The Reparameterization Trick

 reparameterization trick은 <img width="65" alt="image" src="https://user-images.githubusercontent.com/60170358/173177827-0f63fee0-7c08-4c87-acc3-4cce4cdfa377.png">을 추정하기 위한 간단한 approch : encoder에서 나온 확률분포로 바로 sampling로 하면 back propagation이 불가능 (back propagation은 편미분을 구함으로 gradient를 구하는 것인데, z를 확률분포에서 그냥 sampling 한다면 chain rule이 중간에 끊기게 됨, 그래서 reparameterization trick사용) 

<img width="374" alt="image" src="https://user-images.githubusercontent.com/60170358/173178083-e3be983f-4275-4ae8-8897-844b9d37a932.png">

<img width="337" alt="image" src="https://user-images.githubusercontent.com/60170358/173178720-a19d5d6f-d8c8-48c3-b051-058be1e516c2.png">

variational loss function을 위의 식 (13)과 같이 바꿔줌



### 3.2 Example: The Case of Normal Distribution

<img width="48" alt="image" src="https://user-images.githubusercontent.com/60170358/173178931-29865ed8-70ac-4d9b-a01a-fd951cb77c7b.png">를 gaussian 분포로 모사하는게 보통적이지만, 

<img width="282" alt="image" src="https://user-images.githubusercontent.com/60170358/173179005-3a4486e8-37ad-4dcc-8653-477f0e80506d.png">를 통해  reparametrisation trick 를<img width="63" alt="image" src="https://user-images.githubusercontent.com/60170358/173179016-b61d39cd-ce60-4de4-8c09-77bfdbb5bd6b.png">가능하게 해야함

이때 <img width="63" alt="image" src="https://user-images.githubusercontent.com/60170358/173179016-b61d39cd-ce60-4de4-8c09-77bfdbb5bd6b.png">은 normal distribution임

![image-20220611170221631](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20220611170221631.png)

전체 network의 loss function은 식 (15)와 같게 됨



### 3.3 Disentangled Autoencoders

<img width="373" alt="image" src="https://user-images.githubusercontent.com/60170358/173179638-f47478e3-7e71-4dfa-9441-c0c848e453c4.png">

식 (9)에서 오른쪽 항은 sample의 reconstruction 능력을 나타내고, 왼쪽 항은 regularization으로 행동함	

disentangled autoencoder은 varational autoencoder을 포함함->  𝛽는 KL divergence의 곱셈 인자로 추가됨

![image-20220611172110746](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20220611172110746.png)

최대화 계수는 식 (16)과 같음

실제로, 사전 <img width="35" alt="image" src="https://user-images.githubusercontent.com/60170358/173179905-6f1406bc-060c-40ed-bbf4-77de72b71e1e.png">은 일반적으로 standard multivariate normal distribution으로 설정됨 

-> 모든 feature은 상관 관계가 없으며, KL divergence는 latent feature distribution을 상관관계가 낮은 것으로 규제함



## 4 Applications of autoencoders

autoencoder을 통해 표현을 학습하는 것은 다양한 응용 프로그램에 사용될 수 있음



### 4.1 Autoencoders as a generative model

variational autoencoder는 확률적 분포를 통한 데이터 생성을 설명하는 generative model

<img width="379" alt="image" src="https://user-images.githubusercontent.com/60170358/173185555-1e5e0b6b-fa63-4da1-9302-c73d01a3f2fd.png">

variational autoencoder에 의해 생성된 이미지이고, MNIST 데이터 세트에 대해 학습



### 4.2 Use of autoencoders for classification

autoencoder는 unsupervised 방식으로 으로 훈련되고 있지만, 분류 결과를 개선하기 위해 semi-supervised에서도 사용됨

1) autoencoder는 이전 섹션에서 설명한 것처럼 감독되지 않은 방식으로 훈련->  decoder을 한쪽에 두고 encoder을 분류 모델의 첫 번째 부분으로 사용

   support vector machine 인코더의 출력을 훈련

   domain이 high dimensional 이고 layer-by-layer training이 불가능하면, non linearity 를 추가하기 전에 각 계층을 linear layer로 훈련

2) classification newtork에 대한 regularization 기술로 autoencoder을 사용

   classification newtork(라벨이 지정된 데이터로 훈련됨)와 decoder network(라벨이 지정된 데이터 또는 레이블이 지정되지 않은 데이터를 재구성하도록 훈련됨)

   <img width="363" alt="image" src="https://user-images.githubusercontent.com/60170358/173186206-eeca6e15-6a9d-47db-900b-ab9555f85ab5.png">

   autoencoder을 regularization으로 사용



### 4.3 Use of autoencoders for clustering

cluastering은 각 그룹의 샘플이 서로 유사하고 다른 그룹의 샘플과 다르도록 데이터를 그룹으로 분할하는 것이 목표인 unsupervised task-> 대부분의 clustering algorithm은 데이터의 차원에 민감하고 차원의 저주 문제 존재

데이터에 low-dimensional latent representation이 있다고 가정하면, 더 적은 feature로 구성된 데이터에 대한 표현을 계산하기 위해 autoencoder을 사용할 수 있음

autoencoder는 분류에서의 사용과 유사하게 decoder는 한쪽에 놓이고, 각 데이터 포인트의 latent representation(encoder output)이 유지되고 주어진 clustering(K-means)에 대한 입력으로 사용됨



### 4.4 Use of autoencoders for anomaly detection

anomaly detection은 unsupervised task-> 정상 데이터 예제만 주어진 정상 profile을 학습한 다음 정상 profile에 맞지 않는 sample을 이상 징후로 식별

부정 행위 탐지, 시스템 모니터링 등과 같은 다양한 응용 프로그램에 적용될 수 있음



### 4.5 Use of autoencoders for recommendation systems

추천 시스템(Recommendor System)은 사용자 선호도 또는 선호도를 예측하고자 하는 모델 또는 시스템

Recommendor System은 전자 상거래 웹 사이트, 애플리케이션 스토어등에 사용-> 권장 시스템 모델의 고전적인 접근 방식은 Collaborative Filtering(CF) : CF에서 사용자 기본 설정은 다른 사용자 기본 설정의 정보를 기반으로 추론(과거에 유사한 선호도를 보인 사람들은 미래에 비슷한 선호도를 보일 것)

추천 시스템용 autoencoder은 AutoRec model : user-based AutoRec/item-based AutoRec 두가지가 존재: U-AutoRec은 특정 사용자에 대한 lower dimensional representation을 학습하는 반면, I-AutoRec에서는 autoencoder 특정 항목에 대한 lower dimensional representation을 학습



### 4.6 Use of autoencoders for dimensionality reduction

텍스트나 이미지와 같은 real data는 sparse high-dimensional representation을 사용-> 차원성의 저주로 이어짐

차원 축소의 목표는 lower dimensional manifold("intrinsic dimensionalisty") 공간을 배우는것

Principal Component Analysis(PCA): data point를 저차원 공간에 선형 투영하여 sqaure reconstruct loss를 최소화

서로 다른 objectives를 사용: Linear Discriminant Analysis(LDA)은 서로 다른 클래스의 데이터를 구별하는 데 가장 적합한 선형 부분 공간을 찾는 supervised method, ISOMAP은 원래 공간에서 쌍별 데이터 사이의 geodesic distance를 유지하여 저차원 매니폴드를 학습



## 5  Advanced autoencoder techniques

autoencoder은 일반적으로 입력과 출력 사이의 차이에 해당하는 loss function에 의해 훈련-> 위와 같이 autoencoder의 장점 중 하나는 다양한 용도에 latent representation을 사용할 수 있다는 것



### 5.1 Autoencoders and generative adversarial networks

Variational auto encoder는 약간 흐릿한 이미지를 생성하는 MSE에서 훈련되지만 출력을 제어하기 위해 latent variable 추론을 허용

데이터를 합성하는 자동 인코더의 대체 생성 모델은 GAN(Generative Adversarial Networks)-> generator과 discriminator는 서로 경쟁하도록 강제하는 loss function을 사용해 함께 훈련되어 생성 데이터의 품질을 향상시킴

adversarial autoencoder에서, VAE 손실 함수의 KL divergence는 사전과 추정된 사후를 구별하는 discriminator로 대체됨

VAE loss의 reconstruction loss는 discriminator로 대체되며, 이는 decoder을 generator와 병합하도록 함

GAN의 discriminator은 shared weights를 통해 encoder와 결합되어 GMM이 추론을 위해 latent space을 편리하게 모델링함



### 5.2 Adversarially learned inference

GAN의 단점 중 하나는 mode collapse인데, autoencoder와 달리 모든 데이터가 아닌 데이터의 일부만 latent space를 통해 나타낼 수 있음

Adversarially Learned Inference(ALI)에는 VAE와 GANS의 아이디어를 병합하여 장단점을 절충하려는 시도-> decoder를 "속이기" 위해 결과를 출력하도록 강제됨<img width="439" alt="image" src="https://user-images.githubusercontent.com/60170358/173190907-349ef34a-db4b-4af0-8ae3-b30243089934.png">

ALI가 이미지에서 의미 있는 변경을 얻기 위해 특정 feature을 변경하는 방법에 대한 예(모델은 CelebA 데이터 세트에 대해 훈련)

-> 각 이미지에 40개의 서로 다른 속성이 포함되어 있으며, ALI에는 encoder, decoder, discriminator에 선형적으로 포함

-> 각 행에는 여러 열에 걸쳐 일정하게 유지되는 속성의 부분 집합이 존재



### 5.3 Wasserstein autoencoders

GAN은 이미지를 generate하지만 inference를 제공하지 않으며 학습 안정성과 관련하여 많은 고유한 문제를 갖음

Wasserstein-GAN(WGAN)은 Wasserstein distance를 사용하여 많은 문제를 풀 수 있음

Wasserstein distance는 Optimal Transport distance의 특별한 경우이고 <img width="29" alt="image" src="https://user-images.githubusercontent.com/60170358/173232159-688e3c22-4e0e-4a39-a9a2-9296cd5cb173.png">와 <img width="23" alt="image" src="https://user-images.githubusercontent.com/60170358/173232168-1bf11efe-c687-4bab-8b10-a03ecd2260d5.png">사이의 거리를 재는 것

<img width="330" alt="image" src="https://user-images.githubusercontent.com/60170358/173232189-5f6bfcfe-5fa3-4783-85bf-438e4e212e2c.png">

c(X,Y)는 cost function, p-th root of Wc는 p-Wassertein distance라고 부름

WAE(Wasserstein autoencoders)에서 autoencoder의 loss function을 수정하여 식(21) 과 같은 objective function을 만듬

<img width="400" alt="image" src="https://user-images.githubusercontent.com/60170358/173233330-1784012b-3738-4b39-bf79-04d0af8521f7.png">

-> Q는 encoder이고 G는 decoder

-> 왼쪽 부분은 reconstruction loss로 output 분포와 sample 분포 사이를 penalize / 오른쪽 부분은 latent space 분포에서 사전 분포 사이를 penalize

<img width="455" alt="image" src="https://user-images.githubusercontent.com/60170358/173233573-07c3ecd1-92f3-4b65-8ecd-6d48580cc945.png">

-> VAE와 WAE 사이의 정규화 차이

-> VAE와 WAE는 2가지 term을 최소화: reconstruction cost와 regularizer penalizing  discrepancy(encoder Q로 유도된 분포와 Pz사이)

-> 그림 (a) : VAE는 𝑄(𝑍|𝑋 = 𝑥) 를 강제로 Px에서 도출된 모든 입력 예에 대해 Pz와 일치시킴  -> 모든 빨간색 공은 흰색 모양으로 표시된 Pz와 일치해야함 빨간 공들의 intersecting으로 인해 reconstruction 문제가 생김

-> 그림 (b) : WAE는 Pz와 일치시키기 위해 연속적인 mixture 𝑄z:= ∫ 𝑄(𝑍|𝑋)𝑑𝑃x 를 사용-> 초록색 공으로 표시됨, 서로 다른 exmaple의 latent code는  서로 멀리 떨어져 있게 되어 더 나은 reconstruction을 촉진



### 5.4 Deep feature consistent variational autoencoder

autoencoder을 최적화하기 위해 다른 loss function 제시: 원본 영상과 reconstruction 영상이 주어지면 픽셀 차이에서 norm을 측정하는 대신 픽셀 간의 상관 관계를 고려하는 다른 측정이 사용됨

Pretrained classification network는 autoencoder에 대한 loss function을 생성하는데 사용함-> 영상을 encoding 및 decoding 한 후 original과 reconstructed 영상이 모두 pretrained network의 input으로 삽입됨

pretrained network 결과가 높은 정확도로 나타나고 학습된 도메인이 autoencoder의 도메인과 크게 다르지 않다고 가정하면, 각 layer는 input image의 성공적인 feature extractor로 볼 수 있고,  두 이미지 간의 차이를 직접 측정하는 대신 network layer에서 두 이미지 간의 차이를 측정할 수 있음



### 5.5 Conditional image generation with PixelCNN decoders

autoencoder와 PixelCNN사이의 구성을 제안-> PixelCNN에서 이미지의 픽셀은 임의의 순서에 따라 정렬되고,각 픽셀이 이전 픽셀의 출력과 입력의 결과인 출력이 순차적으로 형성됨

image의 local sptial statistics를 고려함(예를 들어, 배경 픽셀 아래에 다른 배경 픽셀을 가질 확률이 전경 픽셀을 가질 확률보다 높음): 입력 픽셀 정보 외에 spatial ordering을 사용하면 흐릿한 픽셀을 얻을 가능성이 줄어듬

이후에, local statistics는 RNN의 사용으로 대체되었지만, 픽셀 생성에 대한 동일한 개념은 남음(decoder을 순차적으로 출력 이미지를 생성하는 픽셀 CNN network로 구성하도록 설정하여 autoencoder와 결합)

<img width="318" alt="image" src="https://user-images.githubusercontent.com/60170358/173235224-a9daeb19-e911-4665-bd97-95c424223769.png">

-> 위-> 아래, 오른쪽-> 왼쪽으로 생성되는 pixelCNN generation network (다음에 생성되는 픽셀은 노란색 픽셀이고, RNN은 숨겨진 상태와 빨간색 사각형의 녹색 픽셀 정보를 고려)



## 6 Conclusion

처음에 정의된 architecture가 입력의 meaningful representation을 학습하고 generative process를 모델링하는 핵심 능력을 가진 강력한 모델로 어떻게 진화했는지를 보여주는 autoencoder을 제시

autoencoder fall backs중 하나는, reconstruction error에 출력이 얼마나 현실적인지 포함하지 않는다는 것

generative prosses를 모델링하는 경우, variational 과 disentangled autoencoder의 성공에도 불구하고 hidden state의 size와 distribution을 선택하는 방법은 실험기반이며, reconstruction error를 고려하고, 사후 훈련에서 hidden state를 변화시킴 -> 이러한 parameter을 더 잘 설정하는 향후 연구가 필요

**autoencoder의 목표는 compressed and meaningful 한 representation을 얻는 것**
