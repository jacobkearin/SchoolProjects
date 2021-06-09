# This program processes user input data for a farming survey and outputs as a new .xlsx file
# data is in project_data_2.xlsx file

# plotting for section a is commented out. uncomment for plots. 
# all print functions are commented.

import pandas as pd
import xlsxwriter as xw
from matplotlib import pyplot as plt


# -- establishing global variables --

path = 'C:/Users/keajacm/Documents/ENGR/programming/'        # change directory path to appropriate location
filename = 'project_data_2.xlsx'

wb = path + 'final_2_output.xlsx'       # naming new workbook named final_1_output with xlsxwriter
wkb = xw.Workbook(wb)
writer = pd.ExcelWriter(wb)

data_frame_1 = pd.read_excel((path + filename), sheet_name='Sheet 1')
#print(data_frame_1)

langtemp = data_frame_1['What Language Do You Speak?'].tolist()
langlist = list(dict.fromkeys(langtemp))        # creates a list of all languages without duplicates
sitetemp = data_frame_1['User IP'].tolist()
sitelist = list(dict.fromkeys(sitetemp))        # creates a list of all IP addresses without duplicates
crops = data_frame_1.columns[1:34]          # creates a list of all crops


# -- a. total number of votes for each crop --
df_a = pd.DataFrame()
for i in crops:
    temp = 0
    for j in data_frame_1.index:
        if i == data_frame_1.at[j, i]:
            temp = temp +1
    df_a.at[i, 'Crop'] = i
    df_a.at[i, 'Votes'] = temp

#print(df_a)
sheetn = 'a.'
worksheet = wkb.add_worksheet(sheetn)
df_a.to_excel(writer, sheetn, index = False)

voteslist = df_a['Votes'].tolist()      # creates a list of the vote counts for each crop
'''plt.bar(range(len(crops)), voteslist)      # uncomment these 6 lines to show bar graph of votes per crop
plt.title("Votes Per Crop")
plt.xticks(range(len(crops)), crops)
plt.xlabel("Crops")
plt.ylabel("Votes")
plt.show()'''


# -- b. number of counts for each language at each site --
df_b = pd.DataFrame()
for i in langlist:      # index of language list
    df_b.at[i, 'language'] = i
    for j in sitelist:      # index of IP list
        df_b.at[i, j] = 0
        lang = 0
        for k in data_frame_1.index:
            if data_frame_1.at[k, 'What Language Do You Speak?'] == i and data_frame_1.at[k, 'User IP'] == j:
                lang = lang + 1         # sums total occurences of each language for each IP, puts it in the df, then resets variable for next sum
                df_b.at[i, j] = lang
#print(df_b)
sheetn = 'b.'
worksheet = wkb.add_worksheet(sheetn)
df_b.to_excel(writer, sheetn, index = False)


# -- c. name, email address and site IP address for each person who said “Yes” to “Would you like to learn more?” --
df_c = pd.DataFrame()
for i in data_frame_1.index:            # checks every submission for yes answer in each language on the survey, then adds data to df if 'yes'
    if data_frame_1.at[i, 'Would you like to learn more?'] == 'Yes!':
        df_c.at[i, 'Name'] = data_frame_1.at[i, 'Name']
        df_c.at[i, 'Email'] = data_frame_1.at[i, 'Email Address']
        df_c.at[i, 'IP'] = data_frame_1.at[i, 'User IP']
    elif data_frame_1.at[i, 'Would you like to learn more?'] == 'si':
        df_c.at[i, 'Name'] = data_frame_1.at[i, 'Name']
        df_c.at[i, 'Email'] = data_frame_1.at[i, 'Email Address']
        df_c.at[i, 'IP'] = data_frame_1.at[i, 'User IP']
    elif data_frame_1.at[i, 'Would you like to learn more?'] == 'Oui!':
        df_c.at[i, 'Name'] = data_frame_1.at[i, 'Name']
        df_c.at[i, 'Email'] = data_frame_1.at[i, 'Email Address']
        df_c.at[i, 'IP'] = data_frame_1.at[i, 'User IP']
    elif data_frame_1.at[i, 'Would you like to learn more?'] == 'はい!':
        df_c.at[i, 'Name'] = data_frame_1.at[i, 'Name']
        df_c.at[i, 'Email'] = data_frame_1.at[i, 'Email Address']
        df_c.at[i, 'IP'] = data_frame_1.at[i, 'User IP']

#print(df_c)
sheetn = 'c.'
worksheet = wkb.add_worksheet(sheetn)
df_c.to_excel(writer, sheetn, index = False)


# -- d. total number of unique submissions by each unique IP address --
df_d = pd.DataFrame()
for i in sitelist:      # index of IP list
    temp = 0
    for j in data_frame_1.index:
        if data_frame_1.at[j, 'User IP'] == i:
            temp = temp + 1         # sums total submissions for each IP address then resets
    df_d.at[i, 'IP'] = i
    df_d.at[i, 'votes'] = temp

#print(df_d)
sheetn = 'd.'
worksheet = wkb.add_worksheet(sheetn)
df_d.to_excel(writer, sheetn, index = False)


# -- e. total votes for each crop by each unique IP Address --
df_e = pd.DataFrame()
for i in crops:     # index of crops list
    df_e.at[i, 'crop'] = i
    for k in sitelist:      # index of IP list
        crop = 0
        for j in data_frame_1.index:
            if data_frame_1.at[j, 'User IP'] == k and data_frame_1.at[j, i] == i:
                crop = crop + 1         # sums total votes for each crop for each IP, puts it in the df, then resets variable for next sum
            df_e.at[i, k] = crop

#print(df_e)
sheetn = 'e.'
worksheet = wkb.add_worksheet(sheetn)
df_e.to_excel(writer, sheetn, index = False)


# -- f. part e but crop votes percentage of the number of people who voted per IP --
df_f = pd.DataFrame()
for i in range(len(crops)):     # integer for index of crops list
    df_f.at[i, 'Crop'] = crops[i]
    for k in sitelist:      # index of IP list
        crop = 0
        for j in data_frame_1.index:
            if data_frame_1.at[j, 'User IP'] == k and data_frame_1.at[j, crops[i]] == crops[i]:
                crop = crop + 1         # sums total votes for each crop for each IP, puts it in the df, then resets sum for next sum
            df_f.at[i, k] = round((crop/(df_d.at[k, 'votes'])*100), 2)

#print(df_f)
sheetn = 'f.'
worksheet = wkb.add_worksheet(sheetn)
df_f.to_excel(writer, sheetn, index = False)


# -- g. part f but sorting each IP by descending percentage --
df_g = pd.DataFrame()
for i in sitelist:
    temp = pd.DataFrame()
    temp = pd.concat([df_f['Crop'], df_f[i]], axis = 1)
    temp = temp.sort_values(by = i, ascending = False)      # creates a new dataframe of crops in descending order of votes using df_f
    tempc = temp['Crop'].tolist()
    templ = temp[i].tolist()        # two new lists of previously made df columns for each language
    #print(templ)
    df_g['Crop ' + str(sitelist.index(i)+1)] = tempc
    df_g[i] = templ

#print(df_g)
sheetn = 'g.'
worksheet = wkb.add_worksheet(sheetn)
df_g.to_excel(writer, sheetn, index = False)


# -- h.d. total number of unique votes by each language --
df_h_d = pd.DataFrame()
for i in langlist:      # index of language list
    temp = 0
    for j in data_frame_1.index:
        if data_frame_1.at[j, 'What Language Do You Speak?'] == i:
            temp = temp + 1     # sums total submissions for each language then resets
    df_h_d.at[i, 'Language'] = i
    df_h_d.at[i, 'votes'] = temp

#print(df_h_d)
sheetn = 'h_d.'
worksheet = wkb.add_worksheet(sheetn)
df_h_d.to_excel(writer, sheetn, index = False)


# -- h_e. total votes for each crop by each language --
df_h_e = pd.DataFrame()
for i in crops:
    df_h_e.at[i, 'crop'] = i
    for k in langlist:
        crop = 0
        for j in data_frame_1.index:
            if data_frame_1.at[j, 'What Language Do You Speak?'] == k and data_frame_1.at[j, i] == i:
                crop = crop + 1
            df_h_e.at[i, k] = crop

#print(df_h_e)
sheetn = 'h_e.'
worksheet = wkb.add_worksheet(sheetn)
df_h_e.to_excel(writer, sheetn, index = False)


# -- h_f. part h_e but crop votes percentage of the number of people who voted per language --
df_h_f = pd.DataFrame()
for i in range(len(crops)):         # integer for each str in crops list
    df_h_f.at[i, 'Crop'] = crops[i]
    for k in langlist:      # index of languages
        crop = 0
        for j in data_frame_1.index:
            if data_frame_1.at[j, 'What Language Do You Speak?'] == k and data_frame_1.at[j, crops[i]] == crops[i]:
                crop = crop + 1         # sums total votes for each crop for each language, puts it in the df, then resets sum for next line
            df_h_f.at[i, k] = round((crop/(df_h_d.at[k, 'votes'])*100), 2)

#print(df_h_f)
sheetn = 'h_f.'
worksheet = wkb.add_worksheet(sheetn)
df_h_f.to_excel(writer, sheetn, index = False)


# -- h_g. part h_f but sorting each language by descending percentage --
df_h_g = pd.DataFrame()
for i in langlist:
    temp = pd.DataFrame()
    temp = pd.concat([df_h_f['Crop'], df_h_f[i]], axis = 1)
    temp = temp.sort_values(by = i, ascending = False)      # creates a new dataframe of crops in descending order of votes using df_h_f
    tempc = temp['Crop'].tolist()
    templ = temp[i].tolist()        # two new lists of previously made df columns for each language
    #print(templ)
    df_h_g['Crop ' + str(langlist.index(i)+1)] = tempc
    df_h_g[i] = templ

#print(df_h_g)
sheetn = 'h_g.'
worksheet = wkb.add_worksheet(sheetn)
df_h_g.to_excel(writer, sheetn, index = False)


writer.save()
