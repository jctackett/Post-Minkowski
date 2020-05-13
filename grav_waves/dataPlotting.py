import pandas as pd
import matplotlib.pyplot as plt

data1 = pd.read_csv('/Users/zackarywindham/Library/Developer/Xcode/DerivedData/Two_Body_Problem-gwaokzfdwakjwzgdakwkjebdxqrs/Build/Products/Debug/first_body_data.csv')
data2 = pd.read_csv('/Users/zackarywindham/Library/Developer/Xcode/DerivedData/Two_Body_Problem-gwaokzfdwakjwzgdakwkjebdxqrs/Build/Products/Debug/second_body_data.csv')

fig = plt.figure()
ax = fig.add_subplot(111,aspect='equal')

data1.columns = ['x', 'y']
data2.columns = ['x', 'y']
ax.plot(data1.x, data1.y) 
ax.plot(data2.x, data2.y)

plt.show()

