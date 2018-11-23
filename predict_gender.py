import sys
import math
import numpy as np

def features(name):
    name = name.lower()
    return {
        'first-letter': name[0], # First letter
        'first2-letters': name[0:2], # First 2 letters
        'first3-letters': name[0:3], # First 3 letters
        'last-letter': name[-1],
        'last2-letters': name[-2:],
        'last3-letters': name[-3:],
    }        
    
def fit(X):
    feature_names = []
    vocab = {}
    for x in X:
        #count = count + 1
        #print("x", type(x), len(x),count)
        for f, v in x.items():
            f = "%s%s%s" % (f, '=', v)
            if f not in vocab:
                feature_names.append(f)
                vocab[f] = len(vocab)

    feature_names.sort()
    vocab = dict((f, i) for i, f in enumerate(feature_names))
        

    return (feature_names, vocab)


def transform( X, vocab):    
    Xa = np.zeros((len(X), 6), dtype=int)

    for i, x in enumerate(X):
        for f, v in x.items():
            f_v = "%s%s%s" % (f, "=", v)
            try:
                Xa[i][features_index()[f]] = vocab[f_v]
            except KeyError:
                pass

    return Xa


def Sigmoid(z):
    try:
        G_of_Z = float(1.0 / float((1.0 + math.exp(-1.0*z))))
    except:
        print("z, math", z)
    return G_of_Z

def Hypothesis(theta, x):
    z = 0
    #print(type(theta), type(x))
    for i in range(6):
        pos = x[i]
        #print("Hypothesis:", i)
        z += theta[pos]
    return Sigmoid(z)

def Cost_Function(X,Y,theta,m):
    sumOfErrors = 0
    for i in range(m):
        xi = X[i]
        hi = Hypothesis(theta,xi)
        if Y[i] == 1:
            error = Y[i] * math.log(hi)
        elif Y[i] == 0:
            error = (1-Y[i]) * math.log(1-hi)
        sumOfErrors += error
    const = -1/m
    J = const * sumOfErrors
    #print( 'cost is ', J )
    return J


def Logistic_Regression_By_Stochastic_Gradient_Descent(X,Y,alpha, theta):
    m = len(Y)
    n = len(features_index()) # here we have 6 features
    best_theta = []
    min_cost = 100.0
    max_score = 0.0
    for i in range(m):
        for idx in range(n):# features
            j = X[i][idx]
            theta[j] = theta[j] - alpha * (Hypothesis(theta, X[i]) - Y[i])  #xij is 1
        if i % 10 == 0:
            #cost = Cost_Function(X,Y,theta,m)
            score = Calculate_Score(theta)
            if score > max_score:
                max_score = score
                best_theta = theta
            #print('cost is', cost)
            #if cost < min_cost:
            #    min_cost = cost
            #    best_theta = theta
    #print("min_cost:", min_cost)
    #print("max_score:", max_score)
    return best_theta
                
def Calculate_Score(theta):
    score = 0
    length = len(X_validation)
    for i in range(length):
        h_value = Hypothesis(theta, X_validation[i])
        if h_value > 0.5:
            prediction = 1
        else:
            prediction = 0        
        answer = Y_validation[i]
        if prediction == answer:
            score += 1

    score = float(score) / float(length)

    #print( 'score:', score )
    return score

def features_index():
    features_index = {
         'first-letter': 0,
        'first2-letters': 1, # First 2 letters
        'first3-letters': 2, # First 3 letters
        'last-letter': 3,
        'last2-letters': 4,
        'last3-letters': 5,
        }
    return features_index        

def get_gender(predict):
    gender = "female"
    if predict == GENDER_MALE:
        gender = "male"
    
    return gender    
    
def do_test(vocabulary):

    fileName_test = sys.argv[1]

    names_test = np.genfromtxt(fileName_test, delimiter = ",", dtype = "U25",
                          autostrip = True) 
                          
    X_test = features(names_test) 
    X_test = transform(X_test, vocabulary)
    length = len(X_test)
    predict = 0

    for i in range(length):
        h_value = Hypothesis(theta, X_test[i])
        if h_value > 0.5:
            predict = 1
        else:
            predict = 0       

        print("{},{}".format(names_test[i], get_gender(predict)))      


if __name__ == "__main__":
    # We're using 70% of the data for training
    TRAIN_SPLIT = 0.8
    GENDER_MALE = 1
    GENDER_FEMALE = 0
    alpha = 0.54
    
    X_train = None
    Y_train = None
    X_validation = None
    Y_validation = None
    X_test = None
    Y_test = None
    Best_theta = None
    
    fileName = "/var/www/html/training_dataset.txt"
    
    g_feature_names = []
    g_vocabulary = {}
       
    names = np.genfromtxt(fileName, delimiter = ",", dtype = "U25",
                      autostrip = True) 
    np.random.shuffle(names)

    features = np.vectorize(features)


    X = features(names[:, 0]) 
    Y = np.array([GENDER_MALE if x == "male" else GENDER_FEMALE for x in names[:, 1]] )

   # creating testing and training set
    X_train, X_validation = X[:int(TRAIN_SPLIT * len(X))], X[int(TRAIN_SPLIT * len(X)):]
    Y_train, Y_validation = Y[:int(TRAIN_SPLIT * len(Y))], Y[int(TRAIN_SPLIT * len(Y)):]

    (g_feature_names, g_vocabulary) = fit(X_train)
    #print("g_feature_names:\n", g_feature_names)
    #print("g_vocabulary:\n", g_vocabulary)
    initial_theta = np.zeros((len(g_feature_names), 1), dtype=np.float64)

    X_validation = transform(X_validation, g_vocabulary)

    iterations = len(X_validation) 


    theta = Logistic_Regression_By_Stochastic_Gradient_Descent(
        transform(X_train, g_vocabulary), Y_train,
        alpha,initial_theta)
        
    do_test(g_vocabulary)