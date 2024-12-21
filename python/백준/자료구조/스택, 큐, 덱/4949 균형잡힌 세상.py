while True:
    string = input()
    
    stack = []
    
    if string == '.':
        break
    
    for char in string:
        if char == "(":
            stack.append("(")
        elif char == "[":
            stack.append("[")
        
        elif char == ")":
            if len(stack) != 0 and stack[-1] == "(":
                stack.pop()
            else:
                stack.append(")")
                break
        elif char == "]":
            if len(stack) != 0 and stack[-1] == "[":
                stack.pop()
            else:
                stack.append("]")
                break
        else:
            continue
        
    if len(stack) == 0:
        print("yes")
    else:
        print("no")