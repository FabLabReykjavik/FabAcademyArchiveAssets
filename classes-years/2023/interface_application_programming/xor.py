#
# xor.py
# Neil Gershenfeld 4/27/23
# XOR PyTorch example
#
import torch
import torch.nn as nn
#
# construct data
#
x = torch.tensor([[0, 0],[0, 1],[1, 0],[1, 1]],dtype=torch.float)
y = torch.tensor([0, 1, 1, 0],dtype=torch.float).view(-1, 1)
#
# define model
#
model = torch.nn.Sequential(
   nn.Linear(2,2),
   nn.Tanh(),
   nn.Linear(2,1),
   )
#
# initialize model
#
def init(layer):
   if type(layer) == torch.nn.Linear:
      nn.init.normal_(layer.weight)
model.apply(init)
#
# define training
#
loss = nn.MSELoss(reduction='sum')
optimizer = torch.optim.Adam(model.parameters(),lr=0.01)
#
# training loop
#
for n in range(1000):
   ypred = model(x)
   error = loss(ypred,y)
   if n % 100 == 0:
       print(n,error.item())
   optimizer.zero_grad()
   error.backward()
   optimizer.step()
#
# print output
#
ypred = model(x)
print(ypred.detach().numpy())
