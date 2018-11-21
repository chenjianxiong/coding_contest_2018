"""
This program performs two different logistic regression implementations on two
different datasets of the format [float,float,boolean], one
implementation is in this file and one from the sklearn library. The program
then compares the two implementations for how well the can predict the given 
outcome for each input tuple in the datasets.

@author Per Harald Borgen
"""
import sys
import math
import numpy as np


# scale larger positive and values to between -1,1 depending on the largest
# value in the data


# We're using 70% of the data for training
TRAIN_SPLIT = 0.7
dtype = np.float64
MAX_VALUE = dtype(0.0)
MIN_VALUE = dtype(0.0)
MEAN_VALUE = dtype(0.0)
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


def normalize(X):
    return (X - MEAN_VALUE) / MAX_VALUE

def transform( X):    
    Xa = np.zeros((len(X), 6), dtype=dtype)

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
    for i in range(len(theta)):
        z += x[i]*theta[i]
    return Sigmoid(z)

##For each member of the dataset, the result (Y) determines which variation of the cost function is used
##The Y = 0 cost function punishes high probability estimations, and the Y = 1 it punishes low scores
##The "punishment" makes the change in the gradient of ThetaCurrent - Average(CostFunction(Dataset)) greater
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

##This function creates the gradient component for each Theta value
##The gradient is the partial derivative by Theta of the current value of theta minus
##a "learning speed factor aplha" times the average of all the cost functions for that theta
##For each Theta there is a cost function calculated for each member of the dataset
def Cost_Function_Derivative(X,Y,theta,j,m,alpha):
    sumErrors = 0
    for i in range(m):
        xi = X[i]
        xij = xi[j]
        hi = Hypothesis(theta,X[i])
        error = (hi - Y[i])*xij
        sumErrors += error
    m = len(Y)
    constant = float(alpha)/float(m)
    J = constant * sumErrors
    return J

##For each theta, the partial differential
##The gradient, or vector from the current point in Theta-space (each theta value is its own dimension) to the more accurate point,
##is the vector with each dimensional component being the partial differential for each theta value
def Gradient_Descent(X,Y,theta,m,alpha):
    new_theta = []
    for j in range(len(theta)):
        CFDerivative = Cost_Function_Derivative(X,Y,theta,j,m,alpha)
        new_theta_value = theta[j] - CFDerivative
        new_theta.append(new_theta_value)
    return new_theta

##The high level function for the LR algorithm which, for a number of steps (num_iters) finds gradients which take
##the Theta values (coefficients of known factors) from an estimation closer (new_theta) to their "optimum estimation" which is the
##set of values best representing the system in a linear combination model
def Logistic_Regression(X,Y,alpha,theta,num_iters):
    min_cost = 100
    save_theta = theta    
    m = len(Y)
    for x in range(num_iters):
        new_theta = Gradient_Descent(X,Y,theta,m,alpha)
        theta = new_theta
        if x % 100 == 0:
            #here the cost function is used to present the final hypothesis of the model in the same form for each gradient-step iteration
            cost = Cost_Function(X,Y,theta,m)
            if cost < min_cost:
                min_cost = cost
                save_theta = theta
            #print( 'theta ', theta )
            #print( 'cost is ', cost))
                
    #print("max score:{}".format(max_score))
    #print("theta:{}".format(save_theta))
    #current_score = Calculate_Score(save_theta)
    return save_theta

##This method compares the accuracy of the model generated by the scikit library with the model generated by this implementation
def Calculate_Score(theta):
    score = 0
    #first scikit LR is tested for each independent var in the dataset and its prediction is compared against the dependent var
    #if the prediction is the same as the dataset measured value it counts as a point for thie scikit version of LR
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
    #the same process is repeated for the implementation from this module and the scores compared to find the higher match-rate
    score = float(score) / float(length)

    #print( 'score:', score )
    return score

initial_theta = [0,0,0,0,0,0]
alpha = 0.1

fileName = "training_dataset.txt";

#names = np.genfromtxt(fileName, delimiter = ",", dtype = ["U25", float], 
#                      converters = {1:lambda s: 1.0 if s.strip() == "male" else 0.0}, 
#                      autostrip = True) 

names = np.genfromtxt(fileName, delimiter = ",", dtype = "U25",
                      autostrip = True) 
#print(names)
np.random.shuffle(names)

#print("======Raw data======\n")
#print("Rey", features("Rey"))
#print("Fidel", features("Fidel"))
#print("Jean-Luc", features("Jean-Luc"))
#print("Jo Ann", features("Jo Ann"))



# clean up data


features = np.vectorize(features)


X = features(names[:, 0]) 
Y = np.array([GENDER_MALE if x == "male" else GENDER_FEMALE for x in names[:, 1]] )

# creating testing and training set
X_train, X_validation = X[:int(TRAIN_SPLIT * len(X))], X[int(TRAIN_SPLIT * len(X)):]
Y_train, Y_validation = Y[:int(TRAIN_SPLIT * len(Y))], Y[int(TRAIN_SPLIT * len(Y)):]

fit(X_train)
MAX_VALUE = dtype(len(g_vocabulary))
MIN_VALUE = dtype(0)
MEAN_VALUE = dtype((MAX_VALUE + MIN_VALUE) * 0.5)

normalize = np.vectorize(normalize)
#print("g_feature_names shape:", type(g_feature_names))
#with open("feature_names.txt", "w") as f:
#    f.write("\n".join(g_feature_names))
#print("g_vocabulary shape:", type(g_vocabulary))    

#with open("vocabulary.txt", "w") as f:
#    for k,v in g_vocabulary.items():
#        f.write("{}=>{}\n".format(k, v))


#transformed = transform(features(["Rey", "Fidel", "Jean-Luc", "Jo Ann"]))
#print("After transformed:\n", transformed )

#normalized = normalize(transformed)
#print("After normalized:\n", normalized )

#print("Beging to train......\n")

#transfromed_X_train = transform(X_train)
#print("transfromed_X_train:\n", transfromed_X_train)
#normalized_X_train = normalize(transfromed_X_train)
#print("normalized_X_train:\n", normalized_X_train)
X_validation = transform(X_validation)
X_validation = normalize(X_validation)
iterations = 1000 
Best_theta = Logistic_Regression(normalize(transform(X_train)),Y_train,alpha,initial_theta,iterations)

def do_test():
    #fileName_test = "test_dataset.txt";
    fileName_test = sys.argv[1]

    names_test = np.genfromtxt(fileName_test, delimiter = ",", dtype = "U25",
                          autostrip = True) 
    X_test = features(names_test) 
    X_test = transform(X_test)
    X_test = normalize(X_test)
    print(X_test.shape)
    length = len(X_test)
    predict = 0
    gender = "female"
    for i in range(length):
        h_value = Hypothesis(Best_theta, X_test[i])    
        if h_value > 0.5:
            predict = 1
        else:
            predict = 0
        if predict == GENDER_MALE:
            gender = "male"
        print("{},{}".format(names_test[i], gender))  


do_test()    
    
                      


