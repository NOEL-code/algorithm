N = int(input())
distances = list(map(int, input().split()))
gas_prices = list(map(int, input().split()))

sum_price = 0
min_price = gas_prices[0]

for i in range(len(distances)):
    if min_price > gas_prices[i]:
        min_price = gas_prices[i]
    
    sum_price += min_price * distances[i]

print(sum_price)
    
    
