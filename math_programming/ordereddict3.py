def sort_by_key(t):
    return t[0]

from collections import OrderedDict

d=OrderedDict()
d['x']=100
d['y']=200
d['z']=300
d['l']=500

for k,v in OrderedDict(sorted(d.items(),key=sort_by_key)).items():
    print(k,v)
