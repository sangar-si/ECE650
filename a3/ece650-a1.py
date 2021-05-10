#!/usr/bin/env python
#!/usr/bin/env python3
import sys
import re


"""
Tasks Tracking:
1. Enter or ctrl+d will exit the program (done)
2. Set limit for the magnitude of input entry (done)
3. Any amount of whitespace is allowed before, in between and after. White spaces are to be ignored (done)
4. Any whitespace entry on its own has to be ignored (done)
5. When a co-ordinate is entered without paranthesis it should raise error Eg (2,2,2,2) is wrong (done)
6. Use proper stream to print output or error (done)
7. space adherence (done)
8. case sensitivity and acceptable characters in street name (done)
9. Overlapping street handling (done)
10. Use stdout for output (done)
11. Empty set should not be printed (done)
12. Check the program in remote eceubuntu (done)
13. rm does not take co-ordinate inputs (done)
14. edges dont repeat. (done)
15. white spaces similar to the pdf (done)
"""

def parseLine(line):
    ip_info = []
    #Identify command
    line = line.strip('\n')
    ip_length = len(line)
    line = line.split(' "', maxsplit = 1) #Changed here
    line[0] = line[0].replace(" ","") #Resubmission
    if line[0] not in ['add', 'mod', 'rm', 'gg']:
        raise Exception('Unknown command. Please try again.')
    ip_info.append(line[0].lower())

    if ip_info[0] == 'gg':
        if ip_length > 2:
            raise Exception('Invalid input format. "gg" is not followed by any characters')
        return ip_info

    #Identify and collect street name
    #Changed here
    try:
        line = line[1].split('" ', maxsplit = 1) #Changed here
    except:
        raise Exception('Invalid input format. Enclose Street name in " ". Please try again.')
    #Changed here    
    acceptable_chars = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ']
    line[0]=line[0].replace('"','')
    for character in list(line[0].lower()):
        if character in acceptable_chars:
            pass
        else:
            raise Exception('Invalid Character in Street Name. Use only Alphabets and spaces')

    ip_info.append(line[0].lower())

    if ip_info[0] == 'rm':
        ip_info[1]=ip_info[1].replace('"','')
        if len(line)>1:
            raise Exception("rm does not take co-ordinate inputs")
        return ip_info
    #Identify and collect co-ordinates
    try:
        line=line[1].replace(" ","")
    except:
        raise Exception("Co-ordinates required.")

    #Co-Ordinate format checking. Even though several while loops are nested, it parses the line only once so it is O(n) not quadratic
    i=0
    chk = 0
    list_1 = list(line)
    while(i<len(list(list_1))):
        if list_1[i] == '(':
            i+=1
            while(list_1[i] in ['0','1','2','3','4','5','6','7','8','9','-']):
                i+=1
                chk = 1
            if list_1[i]==',' and (chk == 1):
                i+=1
                while(list_1[i] in ['0','1','2','3','4','5','6','7','8','9','-']):
                    i+=1
                    chk = 2
                if list_1[i]==')' and (chk == 2):
                    i+=1
                    chk = 0
                else:
                    raise Exception("Invalid co-ordinate entry. Check if the entry is of the format (a,b)(c,d)")
            else:
                raise Exception("Invalid co-ordinate entry. Check if the entry is of the format (a,b)(c,d)")
        else:
            raise Exception("Invalid co-ordinate entry. Check if the entry is of the format (a,b)(c,d)")
    

    line = line.replace(')(',',')
    line = line.replace('(','')
    line = line.replace(')','')
    line_list = list(line.split(','))
    try:
        line_list_int = [float(i) for i in line_list]
    except:
        raise Exception("Invalid format for co-ordinates. Please try again.")
    if len(line_list_int)%2 == 1:
        raise Exception("Invalid co-ordinates entry. Please try again")

    
    #Form co-ordinates as list of tubles from the list of numbers
    i=0
    line_list_tuple = []
    
    #CHANGED HERE
    while(i<len(line_list_int)-1):
        if (line_list_int[i]>9999) or (line_list_int[i+1]>9999):
            raise Exception('Co-ordinate value out of range. It is greater than 9999. Please Retry')
        line_list_tuple.append(tuple([line_list_int[i],line_list_int[i+1]]))
        i = i+2
    ip_info.append(line_list_tuple)    
    return ip_info
    #CHANGED HERE
    
def get_key(val,dict1):
    # Credits : https://www.geeksforgeeks.org/python-get-key-from-value-in-dictionary/ for the idea
    for key,value in dict1.items():
        if value == val:
            return key

def build_vertices(vertices,intersections):
    ''' vertices --> the current list of vertices
        intersections --> updated list of intersections
        This function updates the dictonary that maintains the list of vertices'''
    keys = list(vertices.keys())
    items = list(vertices.values())
    to_delete = []
    to_add = []
    if len(keys) == 0:
        for key_var,intersection in zip(range(len(set(intersections))),set(intersections)):
            vertices[key_var+1] = intersection
    else:
        for dict_item in items:
            if dict_item not in intersections:
                to_delete.append(dict_item)
        for intr in intersections:
            if intr not in items:
                to_add.append(intr)
    for k in keys:
        if vertices[k] in to_delete:
            if len(to_add)>0:
                vertices[k] = to_add[0]
                to_add.pop(0)
            else:
                vertices.pop(k)
    
    while(len(to_add)>0):
        max_key = max(list(vertices.keys()))
        vertices[max_key+1] = to_add[0]
        to_add.pop(0)
    return vertices

def addStreet(street_dict, ip_info):
    #Adds the entered information to the street dictionary
    #Street_dict --> Dictionary that maintains the details of all the entered streets
    #ip_info --> List containing the entered details
    if ip_info[1] in street_dict.keys():
        raise Exception('Invalid Operation. Addition failed. Street already exists in database.')
    street_dict[ip_info[1]] = ip_info[2]
    #print('Street: '+ip_info[1]+' added to database')
    return

def modStreet(street_dict, ip_info):
    #Adds the entered information to the street dictionary
    #Street_dict --> Dictionary that maintains the details of all the entered streets
    #ip_info --> List containing the entered details
    if ip_info[1] not in street_dict.keys():
        raise Exception('Invalid Operation. modification failed. Street does not exist in database.')
    street_dict[ip_info[1]] = ip_info[2]
    #print('Street: '+ip_info[1]+' modified in database')
    return

def rmStreet(street_dict, ip_info):
    #Adds the entered information to the street dictionary
    #Street_dict --> Dictionary that maintains the details of all the entered streets
    #ip_info --> List containing the entered details
    if ip_info[1] not in list(street_dict.keys()):
        raise Exception('Invalid Operation. deletion failed. Street does not exist in database.')
    street_dict.pop(ip_info[1])
    #print('Street: '+ip_info[1]+' deleted from database')
    return

def fdistance(x,x1):
    #returns eucledian distance
    return ((x1[0]-x[0])**2 + (x1[1]-x[1])**2)**0.5

def find_intersection(x,x1,x2,x3,same_street):
    #Reference : Example shared in the ECE650 class repository
    #Check for subsequent lines and same line
    pts = [x,x1,x2,x3]
    if len(set(pts))==3:
        for pt in pts:
            if pts.count(pt)==2:
                return pt,pts,not(same_street),False
    if len(set(pts))<3:
        if same_street:
            return ('nan','nan'),pts,False,False
        else:
            return ('nan','nan'),pts,False,True
    #Intersection calculation
    xnum = ((x[0]*x1[1]-x[1]*x1[0])*(x2[0]-x3[0]) - (x[0]-x1[0])*(x2[0]*x3[1]-x2[1]*x3[0]))
    xden = ((x[0]-x1[0])*(x2[1]-x3[1]) - (x[1]-x1[1])*(x2[0]-x3[0]))
    if xden == 0:
        xcoor = 'nan'
    else:
        xcoor =  xnum / xden

    ynum = (x[0]*x1[1] - x[1]*x1[0])*(x2[1]-x3[1]) - (x[1]-x1[1])*(x2[0]*x3[1]-x2[1]*x3[0])
    yden = (x[0]-x1[0])*(x2[1]-x3[1]) - (x[1]-x1[1])*(x2[0]-x3[0])

    if yden == 0:
        ycoor = 'nan'
    else:
        ycoor = ynum / yden
    
    
    #To find if the intersection lies between two given points based on the euclidean distance metric
    if xcoor != 'nan' and ycoor != 'nan':
        d1 = fdistance((x[0],x[1]),(x1[0],x1[1]))
        d2 = fdistance((x[0],x[1]),(xcoor,ycoor))
        d3 = fdistance((x1[0],x1[1]),(xcoor,ycoor))
 
        d4 = fdistance((x2[0],x2[1]),(x3[0],x3[1]))
        d5 = fdistance((x2[0],x2[1]),(xcoor,ycoor))
        d6 = fdistance((x3[0],x3[1]),(xcoor,ycoor))        
        #print((xcoor,ycoor),[x,x1,x2,x3],round(d1/(d2+d3),2))
        #if (xcoor,ycoor) in [x,x1,x2,x3]:
         #   return (xcoor,ycoor),[x,x1,x2,x3],False

        if (round((d1/(d2+d3)),2) == 1) and (round((d4/(d5+d6)),2)==1):
            return (xcoor, ycoor),[x,x1,x2,x3],True, False
        else:
            return ('nan','nan'),[x,x1,x2,x3],False, False
    else:
        return ('nan','nan'), [x,x1,x2,x3],False, False

"""
def gg(street_dict):
    list_of_streets = list(street_dict.keys())
    list_of_coords = list(street_dict.values())
    list_of_coords = sum(list_of_coords,[]) #credits https://www.geeksforgeeks.org/python-ways-to-flatten-a-2d-list/ Mayank for the idea
    print(list_of_coords)
    sum_val_nos = 0
    intersections = []
    #This part of the code scares me...
    #Please tell me how I could have done it optimally. Open to learn from you!
    #Idea : List them all out and doubleshift when there is a change in street (after street length in reached)
    for st in range(len(list_of_streets)-1):
        sum_val_nos = sum_val_nos + len(street_dict[list_of_streets[st]])
        for pt in range(len(street_dict[list_of_streets[st]])-1):
            x = street_dict[list_of_streets[st]][pt]
            x1 = street_dict[list_of_streets[st]][pt+1]
            for inner_sts in range(st+1, len(list_of_streets)):
                for inner_pts in range(len(street_dict[list_of_streets[inner_sts]])-1):
                    x2 = street_dict[list_of_streets[inner_sts]][inner_pts]
                    x3 = street_dict[list_of_streets[inner_sts]][inner_pts+1]
                    intr, points,unique = find_intersection(x,x1,x2,x3)
                    if intr[0] != 'nan':
                        if unique:
                            intersections.append(intr,x,x1,x2,x3)
                        else:
                            intersections.append(intr)
    print(intersections)
"""
def gg_2(street_dict, vertices):
    list_of_streets = list(street_dict.keys())
    list_of_coords = list(street_dict.values())
    list_of_coords = sum(list_of_coords,[]) #credits https://www.geeksforgeeks.org/python-ways-to-flatten-a-2d-list/ Mayank for the idea
    total_list = [i for i in list_of_coords]
    #print(list_of_coords)
    split_point = []
    split_pt = 0
    for key in list_of_streets:
        split_pt = split_pt+len(street_dict[key])
        split_point.append(split_pt)
    del split_pt
    intersections = []
    new_mid = [] #Resubmission
    #Common points check
    common_pts = []
    for co_ord in list_of_coords:
        if list_of_coords.count(co_ord) > 1:
            common_pts.append(co_ord)
    
    common_pts = set(common_pts)
    #Better...
    st = 0
    st2 = 1
    insertions = 0
    street_1 = 0
    street_2 = 0
    #Split points are different street entries where we need to do double increment
    while(st<(len(list_of_coords)-1)):
        if(st+1 in split_point):
            #When moving from one street entry to another we append a 'nan' to intersections for later use.
            #We do double increment to avoid choosing wrong co-ordinates when moving between streets in this setting
            st=st+1
            total_list.insert(st+insertions,('nan','nan'))
            insertions+=1
            street_1+=1
        x = list_of_coords[st]
        x1 = list_of_coords[st+1]
        intr_points = {} #Resubmission
        intr_dist = [] #Resubmission
        #st2=st+1
        st2 = 0 #Resubmission
        street_2=0
        while(st2<(len(list_of_coords)-1)):
            if(st2+1 in split_point):
                st2=st2+1
                street_2+=1
            x2 = list_of_coords[st2]
            x3 = list_of_coords[st2+1]
            intr,points,unique,overlap = find_intersection(x,x1,x2,x3,same_street =(street_1 == street_2))
            #print(intr, " ", points," ",unique)
            if (intr[0] != 'nan'):
                if (unique) or (intr in common_pts):
                    #When there is intersection between two different streets
                    #print([intr, points, unique])
                    intersections.append(intr)
                    intersections.append(x)
                    intersections.append(x1)
                    intersections.append(x2)
                    intersections.append(x3)
                    #total_list.insert(st+1+insertions,intr)   #Resubmission
                    intr_points[fdistance(x,intr)]=intr #Resubmission
                    intr_dist.append(fdistance(x,intr)) #Resubmission
                    insertions +=1
                    new_mid.append(intr) #Resubmission
                else:
                    #Intersection between two roads of a same street
                    intersections.append(intr)
                if overlap:
                    intersections.append(x)
                    intersections.append(x1)
            st2=st2+1  
        intr_dist.sort() #Resubmission
        cnt = insertions - len(intr_dist) #Resubmission
        for i in intr_dist: #Resubmission
            total_list.insert(st+1+cnt,intr_points[i])     #Resubmission     
            cnt+=1                                          #Resubmission                
        st=st+1


   # print(list_of_coords)
    #print("")
    #print(total_list)
    #print("")
    #print(intersections)
    vertices = build_vertices(vertices,set(intersections))
    #max_v = max(list(vertices.keys()))
    #print("V",max_v, file = sys.stdout)
    '''
    print("E = {",file=sys.stdout)
    for index in range(len(total_list)-1):

        p1 = total_list[index]
        p2 = total_list[index+1]
        if (p1 not in set(intersections)) or (p2 not in set(intersections)) or (p1[0] == 'nan') or (p2[0] == 'nan') or p1 == p2 :
            continue
        if index < len(total_list)-2:
            print("<",get_key(p1,vertices),",",get_key(p2,vertices),">,",file=sys.stdout)
        else:
            print("<",get_key(p1,vertices),",",get_key(p2,vertices),">",file=sys.stdout)  
    print("}",file=sys.stdout)
    '''

    edges = []
    i=0
    for index in range(len(total_list)-1):

        p1 = total_list[index]
        p2 = total_list[index+1]
        if (p1 not in set(intersections)) or (p2 not in set(intersections)) or (p1[0] == 'nan') or (p2[0] == 'nan') or p1 == p2 :
            continue
        if (p1 in set(new_mid)) or (p2 in set(new_mid)):        #Resubmission
            item = (get_key(p1,vertices),get_key(p2,vertices))  #Resubmission
            edges.append(item)                                  #Resubmission
    sys.stdout.flush()
    junk=0
    while(junk<100):
        junk+=1

    edges = list(set(edges))
    max_v = 0
    for index in edges:
        i+=1
        item_rev = (index[1],index[0])
        if item_rev in edges:
            edges.remove(index)
    i=0
    for index in edges:
        if(index[1]>max_v):
            max_v = index[1]
        if(index[0]>max_v):
            max_v = index[0]
    
    
    print("V",max_v, file = sys.stdout)
    print("E {",end="",file=sys.stdout)
    for index in edges: 
        if i<(len(edges)-1):
            print("<"+str(index[0])+","+str(index[1])+">,", end="",file=sys.stdout)
        else:
            print("<"+str(index[0])+","+str(index[1])+">",end="",file=sys.stdout)
        i+=1

    print("}",file=sys.stdout)
    sys.stdout.flush()
    sys.stdout.flush()

    return vertices
  #  print(vertices)
    #print(points_list)
def main():
    # YOUR MAIN CODE GOES HERE
    street_dict = {}
    vertices = {}
    #street_dict = {'weber':[(2.0,-1.0),(2.0,2.0),(5.0,5.0),(5.0,6.0),(3.0,8.0)],'king':[(4.0,2.0),(4.0,8.0)],'dave':[(1.0,4.0),(5.0,8.0)]}

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    while True:
        #Reading and evaluating input
        ip_info = ['null']
        line = sys.stdin.readline()

        if line.rstrip() == "":
            break
        #print("read a line:", line)
        try:
            ip_info = parseLine(line)
        except Exception as e:
            print("Error: Invalid Input: "+str(e), file=sys.stderr) #Resubmission
        #Operations
        if ip_info[0]=='add':
            try:
                addStreet(street_dict,ip_info)
            except Exception as e:
                print("Error: Invalid Operation: "+str(e), file=sys.stderr) #Resubmission

        elif ip_info[0]=='mod':
            try:
                modStreet(street_dict,ip_info)
            except Exception as e:
                print("Error: Invalid Operation: "+str(e), file=sys.stderr) #Resubmission

        elif ip_info[0]=='rm':
            try:
                rmStreet(street_dict,ip_info)
            except Exception as e:
                print("Error: Invalid Operation: "+str(e), file=sys.stderr) #Resubmission

        elif ip_info[0]=='gg':
            try:
                vertices = gg_2(street_dict, vertices)
            except Exception as e:
                print("Error: Operation Error: "+str(e), file=sys.stderr) #Resubmission

    #print("Finished reading input")

    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()
