# just used to validate the bitshift operations' implementation
# not used in the main code at all

# 4 bits (signed. can represent -8 to 7)
# val = 0b1001 

NUM_BITS=32
calc = [2**i for i in range(NUM_BITS)]
print(calc)

def numToBits(val):
    val_arr = [0]*NUM_BITS
    if val < 0:
        val_arr[NUM_BITS-1] = 1
        val += calc[NUM_BITS-1]

    for i in range(len(val_arr)-1, -1, -1):
        if (val >= calc[i]):
            val_arr[i] = 1
            val -= calc[i]
    
    return val_arr
    # return val_arr[::-1]

# 32 bit
# print(numToBits(67))
# print(numToBits(-67)) # 1111111110111101

def bitsToNum(val_arr):
    reconstruct_int = 0

    is_negative = False
    if val_arr[NUM_BITS-1] == 1:
        reconstruct_int = 0-calc[NUM_BITS-1]

    for i in range(len(val_arr)-2, -1, -1): # skip the MSB
        if (val_arr[i] == 1):
            reconstruct_int += calc[i]

    return reconstruct_int

# arr = numToBits(-5) # 5=0101 -> -5=1011
# print('arr', arr)
# print(bitsToNum(arr))


# for i in range(-2**15, 2**15-1):
#     assert bitsToNum(numToBits(i)) == i