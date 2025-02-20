import math
print("enter the number:")
a=int(input())

def sum(a):

    if(a):
        return a%10 + sum(a//10)
    else:
        return 0

def aver(a):

    return sum(a)/len(str(a))

def prod(a):
    if(a):
        return a%10 * prod(a//10)
    else:
        return 1

def geom_mean(a):

        return prod(a)**(1./len(str(a)))

print ("average="+str(aver(a)))
print("geometric mean="+str(geom_mean(a)))