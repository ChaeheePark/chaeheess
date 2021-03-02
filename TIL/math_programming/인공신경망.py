import matplotlib.pyplot as plt
import numpy as np
import csv

#숫자로 주어지는 y값을 길이 vector_length인 one-hot 벡터로 변환합니다.
def convert_y_to_one_hot_vector(y,vector_length):
    y_vect=np.zeros((len(y),vector_length))

    for i in range(len(y)):
        y_vect[i,y[i]]=1

    return y_vect

#학습 데이터 세트 개수에서 라벨의 신경망 결과가 일치하지 않는 경우를 빼서 정확성을 계산합니다.
def compute_accuracy(y_test,y_pred):
    size=y_test.shape[0]

    count=0
    for i in range(size):
        diff=abs(np.argmax(y_test[i,:])-np.argmax(y_pred[i,:]))

        if diff!=0:
            count+=1

    return 100-count*100.0/size

#활성화 함수로 sigmod를 사용합니다.
def sigmoid(x):
    return 1/(1+np.exp(-x))

#역전파 알고리즘 적용 시 sigmod 함수의 1차 도함수가 필요합니다.
def sigmoid_derivative(x):
    return sigmoid(x)*(1-sigmoid(x))

#피드포워드를 수행합니다.
def feed_forward(x,W1,W2,b1,b2):
    a1=x
    z2=np.dot(W1,a1)+b1
    a2=sigmoid(z2)
    z3=np.dot(W2,a2)+b2
    a3=sigmoid(z3)

    return a1,a2,a3,z2,z3

#신경망을 학습시키는 함수입니다.
def train(X,Y,node_size,max_iteration,learning_rate):
    #초기 가중치 값으로 무작위 값을 사용합니다.
    W2=np.random.random((node_size['output_layer_size'],node_size['hidden_layer_size']))
    W1=np.random.random((node_size['hidden_layer_size'],node_size['input_layer_size']))
    b2=np.random.random(node_size['output_layer_size'])
    b1=np.random.random(node_size['hidden_layer_size'])

    dataset_size=len(Y)
    list_average_cost=[]
    list_accuracy=[]
    count=0

    while count< max_iteration:

        #비어있는 넘파이 배열을 사용합니다.
        dW2=np.zeros((node_size['output_layer_size'],node_size['hidden_layer_size']))
        dW1=np.zeros((node_size['hidden_layer_size'],node_size['input_layer_size']))
        db2=np.zeros(node_size['output_layer_size'])
        db1=np.zeros(node_size['hidden_layer_size'])

        average_cost=0

        #학습 데이터 세트의 특성(x)와 라벨(y)를 사용하여 학습하기 위해 피드포워드와 역전파 알고리즘을 수행합니다.
        for x,y in zip(X,Y):
            #피드포워드를 수행합니다.
            a1,a2,a3,z2,z3=feed_forward(x,W1,W2,b1,b2)

            #역전파 알고리즘을 수행합니다.
            output_layer_error=y-a3
            delta3=-(output_layer_error)*sigmoid_derivative(z3)
            average_cost+=np.linalg.norm((output_layer_error),2)/dataset_size

            hidden_layer_error=np.dot(W2.T,delta3)
            delta2=hidden_layer_error*sigmoid_derivative(z2)

            dW2+=np.dot(delta3[:,np.newaxis],np.transpose(a2[:,np.newaxis]))/dataset_size
            db2+=delta3/dataset_size

            dW1+=np.dot(delta2[:,np.newaxis],np.transpose(a1[:,np.newaxis]))/dataset_size
            db1+=delta2/dataset_size

        #역전파 알고리즘 실행결과를 사용하여 신경망의 가중치와 편향을 업데이트 한다.
        W2+=-learning_rate*dW2
        b2+=-learning_rate*db2
        W1+=-learning_rate*dW1
        b1+=-learning_rate*db1

        #예측을 해보고 정확도를 측정합니다.
        y_pred=predict_y(X,W1,W2,b1,b2)
        accuracy=compute_accuracy(Y,y_pred)

        #반복 시마다 측정된 비용을 리스트에 저장합니다.
        list_accuracy.append(accuracy)
        list_average_cost.append(average_cost)

        #100번 반복 시마다 비용과 정확도를 출력합니다.
        #실행 시 비용과 정확도는 추이를 보는 데 사용합니다.
        if count%100==0:
            print('{}/{} cost:{}, Prediction accuracy: {}%'.format(count,max_iteration,average_cost,accuracy))
        count+=1
    return W1,W2,b1,b2,list_average_cost,list_accuracy

#주어진 테스트 데이터 세트와 가중치, 편향을 사용하여 신경망의 출력을 리턴합니다.
def predict_y(X,W1,W2,b1,b2):
    dataset_size=X.shape[0]
    y=np.zeros((dataset_size,3))

    for i in range(dataset_size):
        a1,a2,a3,z2,z3=feed_forward(X[i,:],W1,W2,b1,b2)
        y[i]=a3
    return y

if __name__=="__main__":
    #csv 파일로부터 데이터를 가져와 가공합니다.
    #붓꽃 품종을 딕셔너리로 정의하여 문자열로 된 라벨을 숫자값 라벨로 변환하는데 사용합니다.
    Species_Dict={'Iris-setosa':0,'Iris-versicolor':1,'Iris-virginica':2}
    X=[]
    Y=[]

    with open('Iris.csv',newline='') as file:
        reader=csv.reader(file)
        try:
            for i,row in enumerate(reader):
                if i>0:
                    #csv로 부터 읽어온 데이터를 리스트에 저장합니다.
                    X.append(np.array(row[0:4],dtype="float64"))
                    Y.append(Species_Dict[row[-1]])
                #데이터가 저장된 리스트를 넘파이 배열로 변환합니다.
            X=np.array(X)
            Y=np.array(Y)
        except csv.Error as e:
            sys.exit('file{},line{}:{}'.format(filename,reader.line_num,e))
    #{0,1,2} 값을 가지는 라벨을 one-hot 인코딩하여 {0 0 1, 0 1 0, 1 0 0}로 변환합니다.
    Y=convert_y_to_one_hot_vector(Y,vector_length=3)

    #데이터 세트를 무작위로 섞습니다.
    s=np.arange(Y.shape[0])
    np.random.seed(0)
    np.random.shuffle(s)

    Y=Y[s]
    X=X[s]

    #학습용 데이터 (X_train, Y_train)와 테스트용 데이터 (X_test,Y_test)를 8:2비율로 사용합니다.
    size=len(Y)
    p=int(size*0.8)

    X_train=X[0:p]
    Y_train=Y[0:p]
    X_test=X[p:]
    Y_test=Y[p:]

    #신경망을 구성하는 레이어의 노드 개수입니다.
    node_size={'input_layer_size':4, 'hidden_layer_size':8, 'output_layer_size':3}

    #역전파 알고리즘에서 사용하는 학습률입니다.
    learning_rate=0.5

    #신경망을 학습시켜서 가중치와 편향을 리턴받습니다.
    W1,W2,b1,b2,list_avg_cost,list_accuracy=train(X_train,Y_train,node_size=node_size,max_iteration=1000,learning_rate=learning_rate)

    #비용과 정확도를 그래프로 그립니다.
    Figure,ax=plt.subplots(1,2)

    ax[0].title.set_text('Average cost')
    ax[1].title.set_text('Accuracy')

    ax[0].plot(list_avg_cost)
    ax[1].plot(list_accuracy)

    ax[0].set_ylabel('Average cost')
    ax[0].set_xlabel('Iteration number')

    ax[1].set_ylabel('Accuracy')
    ax[1].set_xlabel('Iteration number')

    plt.show()

    #테스트 데이터 세트를 사용해서 예측 정확성을 테스트 합니다.
    y_pred=predict_y(X_test,W1,W2,b1,b2)

    print('Prediction accuracy : {}%'.format(compute_accuracy(Y_test,y_pred)))
