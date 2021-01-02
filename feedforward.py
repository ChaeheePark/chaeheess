import numpy as np
#활성화 함수
def sigmoid(x):
    return 1/(1+np.exp(-x))
#피드포워드를 수행하는 함수
def feed_forward(x,W1,W2,b1,b2):
    #입력 레이어
    a1=x
    #히든 레이어
    z2=np.dot(W1,a1)+b1
    a2=sigmoid(z2)
    #출력 레이어
    #a3에 인공신경망의 출력이 저장된다.
    z3=np.dot(W2,a2)+b2
    a3=sigmoid(z3)

    return a1,a2,a3,z2,z3
#신경망은 총 3개의 레이어로 구성되며 입력 레이어의 노드 개수 3, 히든 레이어 개수3, 출력 레이어 개수1
node_size={'input_layer_size':3, 'hidden_layer_size':3, 'output_layer_size':1}
#초기 가중치 값으로 작위 값을 사용
#각 넘파이 배열 생성시 아큐먼트에 포함되는 레이어 순서를 봐주면 된다.
W2=np.random.random((node_size['output_layer_size'],node_size['hidden_layer_size']))
W1=np.random.random((node_size['hidden_layer_size'],node_size['input_layer_size']))
b2=np.random.random(node_size['output_layer_size'])
b1=np.random.random(node_size['hidden_layer_size'])
#학습 데이터 세트 (특성 X, 라벨Y)
X=np.array([[1,0,0],[0,0,1],[0,1,1],[1,0,1],[1,1,0],[0,1,0],[1,1,1]])
Y=np.array([1,0,0,0,1,1,0])
#특성 하나인 x에 대해 피드포워드로 수행
#라벨 하나인 y는 비용계산을 위해 사용
for x,y in zip(X,Y):
    #특성과 가중치를 사용하여 피드포워드를 수행하고 결과를 리턴받는다.
    a1,a2,a3,z1,z2=feed_forward(x,W1,W2,b1,b2)
    #신경망의 출력 a3와 라벨 y로부터 비용을 계산한다.
    #l2 Norm의 계산을 위해 넘파이에서 제공하는 함수를 사용
    print('a3={},y={},Error(L2 Norm)={}'.format(a3,y,np.linalg.norm((y-a3),2)))
