import uproot
import numpy as np
import re
import os
import PackingHelper as pkg

# Single photon sample
root = uproot.open("/eos/user/t/tsculac/BigStuff/HGCAL/V16_data_ntuples_15June2024/SinglePhotonPU0V16.root")

# Single pion sample
#root = uproot.open("/eos/user/t/tsculac/BigStuff/HGCAL/V16_data_ntuples_15June2024/SinglePionPU0V16.root")

tree = root.get("l1tHGCalTriggerNtuplizer/HGCalTriggerNtuple")

selected_branches = ["good_ts_energy","good_ts_layer","event","good_ts_waferu","good_ts_waferv","good_ts_z"] #ts_* refer to Modules or Module sums (MS)
branches = tree.arrays(selected_branches)

event = 101858 #Event with a nice photon in Sector 0 from the SinglePhoton sample
#event = 19677 #Event with a nice pion from the SinglePion sample

       
 
def create_energy_dict(layer, ts_u, ts_v, ts_en, endcap, sector = 0):
    """
    Creates a dictionary with keys (layer, (u, v)) and energy values.

    Args:
        layer (list or array-like): List of layers.
        ts_u (list or array-like): List of u-coordinates.
        ts_v (list or array-like): List of v-coordinates.
        ts_en (list or array-like): List of energy values.
        endcap (list or array-like): List of valid endcap indices.
        sector (int, optional): Sector to filter by. Default is 0.

    Returns:
        dict: A dictionary with keys (layer, (u, v)) and corresponding energy values.
    """
    # Validate inputs
    if not (len(layer) == len(ts_u) == len(ts_v) == len(ts_en)):
        raise ValueError("Input arrays must have the same length.")
    if not isinstance(endcap, (list, np.ndarray, set, tuple)):
        raise TypeError("endcap must be a list, NumPy array, or similar iterable.")

    # Initialize result dictionary
    result_dict = {}

    # Iterate over the input arrays
    for i, lay in enumerate(layer):
        if i in endcap:  # Check if the index is in the given endcap
            # Convert (u, v) labels from CMSSW to (u,v) labels from Pedro's mapping that was used to define our mapping from MS/STC to Stage-1.
            new_ts_u, new_ts_v, sec = pkg.getuvsector(lay, ts_u[i], ts_v[i])
            if sec == sector:
                # Create key and add energy to the dictionary
                key = (lay, (new_ts_u, new_ts_v)) # Keep the value as its original type
                result_dict[key] = ts_en[i]       # Keep the value as its original type
#                key = str((lay, (new_ts_u, new_ts_v))) # Convert the value to string
#                result_dict[key] = str(ts_en[i])       # Convert the value to string

    return result_dict
    
    

def extract_data_CE_E(line):
    """
    Extracts the key (layer, (u, v)) and board index from a line in a text file.

    Args:
        line (str): A line of text containing the board, channel, layer, and (u, v) values.

    Returns:
        tuple: A tuple with the key (layer, (u, v)) and board index if the line matches the pattern;
               otherwise, returns (None, None).
    """
    match = re.match(r'Board_(-?\d+),Channel_(-?\d+)=Layer_(-?\d+),\s*\((-?\d+),(-?\d+)\)\s*silicon', line)
    
    if match:
        try:
            layer = int(match[3])  # Extract layer and convert to integer
            u = int(match[4])      # Extract u-coordinate and convert to integer
            v = int(match[5])      # Extract v-coordinate and convert to integer
            board_index = int(match[1])  # Extract board index and convert to integer
            
            key = (layer, (u, v))      # Construct the key as (layer, (u, v)) # Keep the value as its original type or convert to string?
#            key = str((layer, (u, v))) # Construct the key as (layer, (u, v)) # Keep the value as its original type or convert to string?
            return key, board_index # Return the key and the board index
        
        except ValueError:
            # If there is a problem with conversion, return None, None
            return None, None
    
    return None, None  # Return None if the line doesn't match the expected pattern



def process_energy(dict, key):
    """
    Helper function to process energy and packed energy from the given dictionary and key.
    
    Arguments:
    - dict: The dictionary containing energy values.
    - key: The key used to look up the energy value in the dictionary.
    
    Returns:
    - energy: The energy value for the given key, or 0 if the key is not found.
    - packed_energy: The packed energy value in the 5E3M format, or 0 if the key is not found.
    """
    try:
        energy = dict[key]
        packed_energy = pkg.pack5E3M_FromInt(pkg.packInt_FromFloat(float(energy)))
    except KeyError:
        energy = 0
        packed_energy = pkg.pack5E3M_FromInt(pkg.packInt_FromFloat(0.))
    return energy, packed_energy



def make_board_files_CE_E(file_path, dict, directory, sector=0):
    """
    Generate board files with packed energy data in binary format.

    Processes a mapping file to retrieve energy values from a dictionary, packs 
    them into 8-bit format, and writes them to sector-specific board files.

    Parameters:
    file_path : str
        Path to the input mapping file.
    dict : dict
        Dictionary containing energy values keyed by (layer, (u, v)).
    directory : str
        Output directory for the generated board files.
    sector : int, optional
        Identifier for the sector, used in file naming (default: 0).
    """    
    # Ensure the directory exists
    if not os.path.exists(directory):
        os.makedirs(directory)
        
    # Open the input file for reading
    with open(file_path, "r") as input_file:
        energies = []
        packed_energies = []
        write_file = 0  # Current board to write
        
        for line in input_file:
            line = line.replace(" ", "").replace("\t", "")
            key, board = extract_data_CE_E(line)  # Extract (layer, (u, v)) and board index

            if board is None:  # Ensure 'board' is not None
                continue
                
            if board == write_file:
                energy, packed_energy = process_energy(dict, key)
                energies.append(energy)
                packed_energies.append(packed_energy)
            else:
                # Write the current board to a file
                with open(f"{directory}/Sector_{sector}_Board_{write_file}.txt", "w") as output_file:
                    for energy in packed_energies:
                        output_file.write(f"{energy:08b}\n")

                # Reset for the next board
                write_file = board
                energies = []
                packed_energies = []

        # Write the last board
        with open(f"{directory}/Sector_{sector}_Board_{write_file}.txt", "w") as output_file:
            for energy in packed_energies:
                output_file.write(f"{energy:08b}\n")



#########################
### Main part of the code
#########################

event_index = pkg.get_index(branches["event"], event)[0]
u = np.asarray(branches["good_ts_waferu"][event_index]).flatten()
v = np.asarray(branches["good_ts_waferv"][event_index]).flatten()
energy = np.asarray(branches["good_ts_energy"][event_index]).flatten()
layer = np.asarray(branches["good_ts_layer"][event_index]).flatten()
index_endcap = pkg.get_endcap_index(branches["good_ts_z"][event_index], 1)

dict_energy_CEE_sector_0 = create_energy_dict(layer, u, v, energy, index_endcap, 0)
dict_energy_CEE_sector_1 = create_energy_dict(layer, u, v, energy, index_endcap, 1)
dict_energy_CEE_sector_2 = create_energy_dict(layer, u, v, energy, index_endcap, 2)

make_board_files_CE_E("inputs/mapping/v2/Input_CEE_v2.txt", dict_energy_CEE_sector_0, "output/Stage1_Unpacker/CEE", 0)
make_board_files_CE_E("inputs/mapping/v2/Input_CEE_v2.txt", dict_energy_CEE_sector_1, "output/Stage1_Unpacker/CEE", 1)
make_board_files_CE_E("inputs/mapping/v2/Input_CEE_v2.txt", dict_energy_CEE_sector_2, "output/Stage1_Unpacker/CEE", 2)