from _bst import *

l= [(10,10),
	(79,79),
	(13,13),
	(80,80),
	(60,60),
	(50,50)]
	
bst = Bst_sizet(l)
print("contents before balancing...")
bst.print()
bst.detailedPrint()
print("\ntree size is " + str(bst.size()))
print("the depth is "+str(bst.depth()))
print("the average depth is "+str(bst.avgdepth()))

print("\nIs the tree balanced? ")
print(bool(bst.checkBalanced()))

print("\nbalancing...")
bst.balance()
print("Is the tree balanced? ")
print(bool(bst.checkBalanced()))

print("\ncontents after balancing...")
bst.print()
bst.detailedPrint()

print("\ntree size is " + str(bst.size()))
print("the depth is "+str(bst.depth()))
print("the average depth is "+str(bst.avgdepth()))

print("\nadding element (8,8)")
bst.insert((8,8))
bst.detailedPrint()

print("\nremoving element (8,8)")
bst.erase(8)
bst.detailedPrint()

print("\nTrying to raise an exception deleting element 11")
try:
	print("trying to remove element (11,11)")
	bst.erase(11)
except Exception as e:
	print(e)

print("\nUsing subscript operators:")	

print("getting existing item. for key 79 the value is:")
print(bst[79])
print("getting non existing item. for key 30 the value is defaulted to:")
print(bst[30])
bst.print()
print("setting item. for key 30 the value is set to 30:")
bst[30]=30
bst.detailedPrint()
