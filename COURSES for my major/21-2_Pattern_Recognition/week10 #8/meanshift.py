import numpy as np


def calc_euclidean_distance(p1, p2):
    return np.sqrt(np.sum((p1 - p2) ** 2))


def calc_weight(x, kernel='flat'):
    if x <= 1:
        if kernel.lower() == 'flat':
            return 1
        elif kernel.lower() == 'gaussian':
            return np.exp(-1 * (x ** 2))
        else:
            raise Exception("'%s' is invalid kernel" % kernel)
    else:
        return 0

    
def mean_shift(X, bandwidth, n_iteration=20, epsilon=0.001):
    centroids = np.zeros_like(X)   

    for i in range(len(X)):        
        centroid = X[i].copy()  # 초기 중심점(t_0) 설정 -> 각 datapoint를 초기 중심점으로 할당
        prev = centroid.copy()
        
        t = 0
        while True:
            
            if (t>n_iteration): #종료조건1) t가 최대 반복 횟수를 초과한 경우
                break

            #현재 중심점으로부터 bandwidth 내에 있는 샘플들을 기반으로 새로운 군집 중심점 계산
            numerator=0
            denominator=0
            for sample in X:
                distance=calc_euclidean_distance(centroid,sample)
                weight=calc_weight(distance/bandwidth,'flat') #반경안에있는 점들만 참여
                numerator+=((sample-centroid)*weight)
                denominator+=weight

            if denominator==0:
                shift=0
            else:
                shift=numerator/denominator

            centroid+=shift

            #종료조건2) 수렴했으면 stop
            if calc_euclidean_distance(centroid,prev)<=epsilon:#중심점 이동이 수렴했을 경우
                break
                
            prev = centroid.copy()
            t += 1
            
        centroids[i] = centroid.copy()

    return centroids

    
def mean_shift_with_history(X, bandwidth, n_iteration=20, epsilon=0.001):
    history = {}
    for i in range(len(X)):
        history[i] = []
    centroids = np.zeros_like(X)   

    for i in range(len(X)):
        centroid = X[i].copy()  # 초기 중심점(t_0) 설정 -> 각 datapoint를 초기 중심점으로 할당
        prev = centroid.copy()
        history[i].append(centroid.copy())
        
        t = 0
        while True:
            
            if (t>n_iteration): #t가 최대 반복 횟수를 초과한 경우
                break
            
            a=0 
            b=0           
            for j in X:
                a+=((j-centroid)*calc_weight(calc_euclidean_distance(centroid,j)/bandwidth, 'flat'))
                b+=calc_weight(calc_euclidean_distance(centroid,j)/bandwidth, 'flat')
            
            if b==0:
                shift=0
            else:
                shift=a/b
            centroid+=shift
                
            if calc_euclidean_distance(centroid,prev)<=epsilon:#중심점 이동이 수렴했을 경우
                break

            prev = centroid.copy()
            t += 1

            history[i].append(centroid.copy())
        
        centroids[i] = centroid.copy()

    return centroids, history
