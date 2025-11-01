string = input()

result = []
stack = []


for c in string:
    if c in ('+', '-', '*', '/'):
        if c in ('+', '-'):
            while stack and stack[-1] != '(' and stack[-1] in ('+','-','*','/'):
                result.append(stack.pop())
            stack.append(c)
        else:  # c in ('*','/')
            while stack and stack[-1] != '(' and stack[-1] in ('*','/'):
                result.append(stack.pop())
            stack.append(c)
    
    elif c == '(':
        stack.append('(')
        
    elif c == ')':
        while stack and stack[-1] != '(':
            result.append(stack.pop())
        stack.pop()
        
    else: 
        result.append(c)
    
while stack:
    result.append(stack.pop())
        
print("".join(result))