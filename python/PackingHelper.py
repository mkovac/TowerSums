import numpy as np


###############################################################
### Functions for packing and unpacking data to wanted format
###############################################################

def pack4E4M_FromInt(energy): #Take input integer and pack it into 4E4M format
    assert(energy<0x80000);#make sure input is an 19 bit number

    if(energy<16): #here the max number is 16 because mantissa has 4 bits, 2^4=16
        return int(energy)
    e = 1
    while(energy>=32):
        e+=1
        energy>>=1
    return int(16*(e-1)+energy) #format it in as 16 bits = eeeemmm, where first 4 are exponent and last 4 are (mantissa - 16)



def unpack4E4M_ToInt(num): #Take 4E4M as input and unpack it to integer
    assert(num<0x100);#make sure input is an 8 bit number

    e = ((num>>4)&0x0f);#Read first 4 bits from the input number to read the exponent
    m = ((num   )&0x0f);#Read the last 4 bits as mantissa
    #print(e,m)

    if(e==0):
        return m
    elif(e==1):
        return 16+m
    else:
        return (32+2*m+1)<<(e-2)#Recover the fact that this is (mantissa - 8), take into account first multiplication with 2 inside the bracket and then shift to right e-2 time which is equivalent to multiplying by 2



def pack5E3M_FromInt(energy):
    """
    Convert an integer to an 8-bit 5E3M representation.

    Parameters:
    energy : int
        Non-negative integer (less than 2^34).

    Returns:
    int
        8-bit integer in 5E3M format (5 bits for exponent, 3 bits for mantissa).
    """
    assert energy >= 0, "Energy must be a non-negative integer."
    assert energy < 0x400000000, "Energy must fit within 34 bits."
    
    # Handle small values directly. The maximum value for direct representation is 8, as the mantissa uses 3 bits (2^3=8).
    if energy < 8:
        return int(energy)
    
    # Determine the exponent and adjust the energy.
    e = 1
    while energy >= 16:
        e += 1
        energy >>= 1
    
    # Combine exponent and mantissa into 8 bits (5E3M format).
    # The 8-bit format is structured as eeeeemmm, where the first 5 bits are the exponent, and the last 3 bits represent the mantissa (with an offset of 8).
    return int(8 * (e - 1) + energy)



def unpack5E3M_ToInt(num): #Take intiger as input and pack it into 5E3M format
    assert(num<0x100);#make sure input is an 8 bit number

    e = ((num>>3)&0x1f);#Read first 5 bits from the input number to read the exponent
    m = ((num   )&0x07);#Read the last 3 bits as mantissa
    #print(e,m)

    if(e==0):
        return m
    elif(e==1):
        return 8+m
    else:
        return (16+2*m+1)<<(e-2)#Recover the fact that this is mantissa - 8, take into account first multiplication with 2 inside the bracket and then shift to right e-2 time which is equivalent to multiplying by 2



def pack5E4M_FromInt(energy): #Take 5E4M format and unpack it into intiger
    assert(energy<0x800000000);#make sure input is an 35 bit number

    if(energy<16): #here the max number is 16 because mantissa has 4 bits, 2^4=16
        return int(energy)
    e = 1
    while(energy>=32):
        e+=1
        energy>>=1
    #print(e,energy)
    return int(16*(e-1)+energy) #format it in as 9 bits = eeeeemmmm, where first 5 are exponent and last 4 are (mantissa - 8)



def unpack5E4M_ToInt(num): #Take intiger as input and pack it into 5E4M format
    assert(num<0x200);#make sure input is an 9 bit number

    e = ((num>>4)&0x1f);#Read first 5 bits from the input number to read the exponent
    m = ((num   )&0x0f);#Read the last 4 bits as mantissa
    #print(e,m)

    if(e==0):
        return m
    elif(e==1):
        return 16+m
    else:
        return (32+2*m+1)<<(e-2)#Recover the fact that this is (mantissa - 8), take into account first multiplication with 2 inside the bracket and then shift to right e-2 time which is equivalent to multiplying by 2



def packInt_FromFloat(energy, constant=10**-8):
    """
    Convert a floating-point energy value to an integer by scaling and rounding.

    Parameters:
    energy : float
        The energy value to convert.
    constant : float, optional
        Scaling factor (default: 10**-8).

    Returns:
    int
        The scaled and rounded integer.
    """
    return int(0.5 + energy / constant)



def unpackFloat_FromInt(energy, constant=10**-8):
    unpacked_energy = float((energy-0.5)*constant)
    if  unpacked_energy > 0.:
        return unpacked_energy
    else:
        return 0.



def undo_trimming(inputData, targetNumberBits, maxNumberBits):
    criticalBitNumber = maxNumberBits - targetNumberBits
    return inputData << criticalBitNumber


###########################################################################################################################
### Functions for converting (u,v) coordinates from the mappings used in mapping files versus those in the CMSSW simulation
###########################################################################################################################

def Sector0(layer, u, v):
    """
    A function that checks if a given (layer, u, v) combination satisfies the conditions
    for being part of Sector0 based on specific rules for different layers.
    
    Args:
        layer (int): The layer number.
        u (int): The u-coordinate.
        v (int): The v-coordinate.

    Returns:
        bool: True if the condition for Sector0 is met, False otherwise.
    """
    # Conditions for layers less than 34, excluding 28, 30, and 32
    if layer < 34 and layer not in {28, 30, 32}:
        if v - u > 0 and v >= 0:
            return True

    # Conditions for even layers greater than or equal to 34
    elif layer >= 34 and layer % 2 == 0:
        if v - u > 0 and v > 0:
            return True

    # Conditions for odd layers greater than or equal to 34
    elif layer >= 34 and layer % 2 == 1:
        if v - u >= 0 and v >= 0:
            return True

    # Special conditions for layers 28, 30, or 32
    elif layer in {28, 30, 32}:
        if u - 2 * v < 0 and u + v >= 0:
            return True

    # If no condition is met, return False
    return False



def getuvsector(layer,u,v):
    """
    Determine the (u, v) coordinates and sector based on layer and Sector0 checks.

    Args:
        layer (int): The layer number.
        u (int): The u-coordinate.
        v (int): The v-coordinate.

    Returns:
        tuple: Transformed (u, v) coordinates and sector number (0, 1, 2).
    """
    if u == -999:
        return (u, v, 0)

    # Helper function for layer checks
    def is_special_layer(layer):
        return layer in {28, 30, 32}

    # Perform Sector0 check and transformations
    def transform_for_sector0(layer, u, v, sector):
        if is_special_layer(layer):
            if u >= 0:
                return (v, u, sector)
            else:
                return (-u, v - u, sector)
        else:
            return (v - u, v, sector)

    # Check Sector0 initially
    if Sector0(layer, u, v):
        return transform_for_sector0(layer, u, v, 0)

    # Apply transformations for sector > 0
    if layer < 34:
        u, v = -v, u - v
    elif layer % 2 == 0:
        u, v = -v + 1, u - v + 1
    else:
        u, v = -v - 1, u - v - 1

    # Check Sector0 after transformation
    if Sector0(layer, u, v):
        return transform_for_sector0(layer, u, v, 1)

    # Apply further transformations for sector == 2
    if layer < 34:
        u, v = -v, u - v
    elif layer % 2 == 0:
        u, v = -v + 1, u - v + 1
    else:
        u, v = -v - 1, u - v - 1

    # Final Sector0 check
    if Sector0(layer, u, v):
        return transform_for_sector0(layer, u, v, 2)

    # If no conditions are met
    return (u, v, -1)  # -1 to signify invalid or unprocessed sector



def getuvSTCidxsector(layer,u,v,cellu,cellv):
    if u == -999:
        return (u,v,0,0)
    if Sector0(layer,u,v):
        if (layer != 28) and (layer != 30) and (layer != 32):
            return(v-u,v,0,0)
        else:
            if u >= 0:
                return (v,u,0,1)
            else:
                return(-u,v-u,0,1)
    else:
        if  (layer <34):
            u,v = -v,u-v
        if (layer >= 34) and (layer%2 == 0):
            u,v = -v+1,u-v+1
        if (layer >= 34) and (layer%2 == 1):
            u,v = -v-1,u-v-1
        if Sector0(layer,u,v):
            if (layer != 28) and (layer != 30) and (layer != 32):
                return(v-u,v,0,1)
            else:
                if u >= 0:
                    return (v,u,0,1)
                else:
                    return(-u,v-u,0,1)

        else:
            if  (layer <34):
                u,v = -v,u-v
            if (layer >= 34) and (layer%2 == 0):
                u,v = -v+1,u-v+1
            if (layer >= 34) and (layer%2 == 1):
                u,v = -v-1,u-v-1
            if Sector0(layer,u,v):
                if (layer != 28) and (layer != 30) and (layer != 32):
                    return(v-u,v,0,2)
                else:
                    if u >= 0:
                        return (v,u,0,1)
                    else:
                        return(-u,v-u,0,1)



#################################################################################
### Functions for converint link index from S1board notation to S2 board notation
#################################################################################

def s1_link_to_s2_link(s1_board, s1_link): #s1_board is index od S1 board form 0 - 13, s1_link is index of link from 0-5. Links 0 and 5 are connected to S2 boards from different sector and links 1,2,3,4 are connected to boards from the same sector
    s2_link = 99
    if(s1_board < 14):
        s2_link = 14*s1_link + s1_board
    return s2_link



#################################################
# Functions for list and dictionary manipulations
#################################################
    
def get_index(array, value):
    """
    Returns all indices in the array where the array has the given value.

    Args:
        array (array-like): Input array or list to search.
        value (any): The value to find in the array.

    Returns:
        numpy.ndarray: Array of indices where the value is found.
    """
    # Convert to NumPy array only if needed
    if not isinstance(array, np.ndarray):
        array = np.asarray(array)
    
    # Find indices where the value matches
    indices = np.where(array == value)[0]
    return indices



def get_endcap_index(array, endcap):
    """
    Returns the indices of the array where values match the given endcap condition.

    Args:
        array (array-like): Input array to check.
        endcap (int): Positive for indices of positive values, negative for indices of negative values.

    Returns:
        numpy.ndarray: Array of indices matching the condition.
    """
    # Convert to NumPy array only if needed
    if not isinstance(array, np.ndarray):
        array = np.asarray(array)

    # Validate the endcap and find indices
    if endcap > 0:
        indices = np.where(array > 0)[0]
    elif endcap < 0:
        indices = np.where(array < 0)[0]
    else:
        indices = np.array([])  # Return an empty array if endcap == 0

    return indices
        
        
        
def eta_index(array):
    """
    Returns 0 if the first element of the array is greater than 0, else 1.

    Parameters:
    array (array-like): Input array to check.

    Returns:
    int: 0 if first element > 0, 1 otherwise.

    Raises:
    ValueError: If the input array is empty.
    """
    numpy_array = np.asarray(array)
    
    if numpy_array.size == 0:
        raise ValueError("Input array is empty")
    
    if numpy_array[0] > 0:
        return 0
    else:
        return 1