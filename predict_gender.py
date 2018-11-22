import sys
import math
import numpy as np


# We're using 70% of the data for training
TRAIN_SPLIT = 0.8
dtype = np.float64
GENDER_MALE = 1
GENDER_FEMALE = 0

X_train = None
Y_train = None
X_validation = None
Y_validation = None
X_test = None
Y_test = None
Best_theta = None


g_feature_names = []
g_vocabulary = {}


   
features_index = {
         'first-letter': 0,
        'first2-letters': 1, # First 2 letters
        'first3-letters': 2, # First 3 letters
        'last-letter': 3,
        'last2-letters': 4,
        'last3-letters': 5,
        }

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
        
    global g_feature_names
    g_feature_names = feature_names
    global g_vocabulary 
    g_vocabulary = vocab


def transform( X):    
    Xa = np.zeros((len(X), 6), dtype=int)

    for i, x in enumerate(X):
        for f, v in x.items():
            f_v = "%s%s%s" % (f, "=", v)
            try:
                Xa[i][features_index[f]] = g_vocabulary[f_v]
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
    for i in range(len(features_index)):
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

def Cost_Function_Derivative(X,Y,theta,j,m,alpha):
    sumErrors = 0
    for i in range(m):
        #print("Cost_Function_Derivative:", i)
        xi = X[i]
        xij = xi[j]
        hi = Hypothesis(theta,X[i])
        error = (hi - Y[i])*xij
        sumErrors += error
    m = len(Y)
    constant = float(alpha)/float(m)
    J = constant * sumErrors
    return J


def Gradient_Descent(X,Y,theta,m,alpha):
    new_theta = []
    for j in range(len(theta)):
        #print("Gradient_Descent:", j)
        CFDerivative = Cost_Function_Derivative(X,Y,theta,j,m,alpha)
        new_theta_value = theta[j] - CFDerivative
        new_theta.append(new_theta_value)
    return new_theta

def Logistic_Regression_By_Stochastic_Gradient_Descent(X,Y,alpha, theta):
    m = len(Y)
    n = len(features_index) # here we have 6 features
#    best_theta = []
#    min_cost = 100
    for i in range(m):
        for idx in range(n):# features
            j = X[i][idx]
            theta[j] = theta[j] - alpha * (Hypothesis(theta, X[i]) - Y[i])  #xij is 1
        #if i % 10 == 0:
        #    cost = Cost_Function(X,Y,theta,m)
            #print('cost is', cost)
       #     if cost < min_cost:
       #         min_cost = cost
       #         best_theta = theta
    #print("min_cost:", min_cost)
    return theta
                
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


fileName = "training_dataset.txt";

names = np.genfromtxt(fileName, delimiter = ",", dtype = "U25",
                      autostrip = True) 

np.random.shuffle(names)

#print("======Raw data======\n")
#print("Rey", features("Rey"))
#print("Fidel", features("Fidel"))
#print("Jean-Luc", features("Jean-Luc"))
#print("Jo Ann", features("Jo Ann"))


features = np.vectorize(features)


X = features(names[:, 0]) 
Y = np.array([GENDER_MALE if x == "male" else GENDER_FEMALE for x in names[:, 1]] )

# creating testing and training set
X_train, X_validation = X[:int(TRAIN_SPLIT * len(X))], X[int(TRAIN_SPLIT * len(X)):]
Y_train, Y_validation = Y[:int(TRAIN_SPLIT * len(Y))], Y[int(TRAIN_SPLIT * len(Y)):]

fit(X_train)

initial_theta = np.zeros((len(g_feature_names), 1), dtype=dtype)
#print("initial_theta:", initial_theta.shape)
#print("g_feature_names shape:", type(g_feature_names))
#with open("feature_names.txt", "w") as f:
#    f.write("\n".join(g_feature_names))
#print("g_vocabulary shape:", type(g_vocabulary))    

#with open("vocabulary.txt", "w") as f:
#    for k,v in g_vocabulary.items():
#        f.write("{}=>{}\n".format(k, v))


#transformed = transform(features(["Rey", "Fidel", "Jean-Luc", "Jo Ann"]))
#print(transformed)
#print("After transformed:\n", transformed )
#np.savetxt('transformed.out', transformed, delimiter=',', fmt='%d')

#print("Beging to train......\n")

#transfromed_X_train = transform(X_train)
#print("transfromed_X_train:\n", transfromed_X_train)
#normalized_X_train = normalize(transfromed_X_train)
#print("normalized_X_train:\n", normalized_X_train)
X_validation = transform(X_validation)
#print(X_validation)
iterations = len(X_validation) 

alpha = 0.54
theta = Logistic_Regression_By_Stochastic_Gradient_Descent(
        transform(X_train), Y_train,
        alpha,initial_theta)

#Calculate_Score(theta)

def do_test():
    #fileName_test = "test_dataset.txt";
    fileName_test = sys.argv[1]

    names_test = np.genfromtxt(fileName_test, delimiter = ",", dtype = "U25",
                          autostrip = True) 
    X_test = features(names_test) 
    X_test = transform(X_test)
    length = len(X_test)
    predict = 0
    gender = "female"
    for i in range(length):
        h_value = Hypothesis(theta, X_test[i])    
        if h_value > 0.5:
            predict = 1
        else:
            predict = 0
        if predict == GENDER_MALE:
            gender = "male"
        print("{},{}".format(names_test[i], gender))  


do_test()                     


