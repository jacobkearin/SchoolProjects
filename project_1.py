# This program processes order data for a farming supply/logistics system
# data is in project_data_1.xlsx file

# plotting for section e. and f. are commented out. uncomment for plots. 
# all print functions are commented. uncomment for debugging

import pandas as pd
import xlsxwriter as xw
from matplotlib import pyplot as plt


# -- establishing global variables --

path = 'C:/Users/keajacm/Documents/ENGR/programming/'        # change directory path to appropriate location
filename = 'project_data_1.xlsx'

wb = path + 'final_1_output.xlsx'       # creating new workbook function with xlsxwriter
wkb = xw.Workbook(wb)
writer = pd.ExcelWriter(wb)

data_frame_1 = pd.read_excel((path + filename), sheet_name='Sheet 1')       # reading all the excel sheets to dataframes with pandas
data_frame_2 = pd.read_excel((path + filename), sheet_name='Sheet 2')
data_frame_3 = pd.read_excel((path + filename), sheet_name='Sheet 3')
#print(data_frame_1)
#print(data_frame_2)
#print(data_frame_3)


# a. -- total quantity sold --
df_a = pd.DataFrame()
total_pounds = 0
for i in data_frame_1.index:
    total_pounds = total_pounds + data_frame_1.at[i, 'Order Quantity']
df_a.at[0, 'Total Pounds Sold'] = total_pounds

#print(df_a)
sheetn = 'a.'
worksheet = wkb.add_worksheet(sheetn)
df_a.to_excel(writer, sheetn, index = False)


# b. -- quantity of each type of crop sold --
df_b = pd.DataFrame()

lot_code_1 = []       # create a list of unique lot code 1 strings
for i in data_frame_3.index:
    temp = data_frame_3.at[i, 'Lot code 1']
    if temp not in lot_code_1:
        lot_code_1.append(temp)

df_b['Crop Name'] = 0       # create a df column for unique crop names, then adds each crop in that column
for i in data_frame_2.index:
    df_b.at[i, 'Crop Name'] = data_frame_2.at[i, 'Crop Name']

for i in lot_code_1:       # create a df column for each lot code(i)
    df_b[str(i)] = 0
    for j in data_frame_2.index:       # checks for each crop type(j)
        pounds_of_crop = 0
        crop_name = data_frame_2.at[j, 'Crop Name']
        for k in data_frame_1.index:       # checks each crop type against every order(k) and saves to df the total pounds in each lot
            if crop_name == data_frame_1.at[k, 'Product'] and i == data_frame_1.at[k, 'Lot code 1']:
                pounds_of_crop = pounds_of_crop + data_frame_1.at[k, 'Order Quantity']
                df_b.at[j, str(i)] = pounds_of_crop

#print(df_b)
sheetn = 'b.'
worksheet = wkb.add_worksheet(sheetn)
df_b.to_excel(writer, sheetn, index = False)


# c. -- total quantity sold to each customer --
df_c = pd.DataFrame()

customers = []       # creates a list of unique customers
for i in data_frame_1.index:
    temp = data_frame_1.at[i, 'Customer']
    if temp not in customers:
        customers.append(temp)

df_c['Customer'] = customers       # writes customers list to df at new column 'Customers'

df_c['Quantity Sold'] = 0.0       # creates new column for qty
for i in range(len(customers)):
    cust_lbs = 0
    for j in data_frame_1.index:       # checks each order for customer name and adds together all qty sold under each customer
        if data_frame_1.at[j, 'Customer'] == customers[i]:
            cust_lbs = cust_lbs + data_frame_1.at[j, 'Order Quantity']
    df_c.at[i, 'Quantity Sold'] = cust_lbs       #writes qty sold to df

#print(df_c)
sheetn = 'c.'
worksheet = wkb.add_worksheet(sheetn)
df_c.to_excel(writer, sheetn, index = False)


# d. -- quantity of each crop to each customer --
df_d = pd.DataFrame()

df_d['Crop Name'] = data_frame_2['Crop Name']

for i in range(len(customers)):       # checks across each customer
    cpc = 0
    for j in data_frame_2.index:       # checks across each crop
        cpc = 0
        prods = data_frame_2.at[j, 'Crop Name']
        for k in data_frame_1.index:       # adds crop qty for each crop for each customer
            if data_frame_1.at[k, 'Product'] == prods and data_frame_1.at[k, 'Customer'] == customers[i]:
                cpc = cpc + data_frame_1.at[k, 'Order Quantity']
        df_d.at[j, customers[i]] = cpc       # writes quantity of each crop of each customer

#print(df_d)
sheetn = 'd.'
worksheet = wkb.add_worksheet(sheetn)
df_d.to_excel(writer, sheetn, index = False)


# e. -- quantity sold each day of the year --
df_e = pd.DataFrame()

order_dates_list = []       #create a list of order dates via the dates codes
for i in data_frame_1.index:
    temp = data_frame_1.at[i, 'Order ID']
    if temp[1:7] not in order_dates_list:
        order_dates_list.append(temp[1:7])
df_e['Order Date'] = order_dates_list

order_dates_qty = []
for i in range(len(order_dates_list)):       #create a list of quantities per order date
    qty_per_date = 0
    for j in data_frame_1.index:
        if data_frame_1.at[j, 'Order ID'][1:7] == order_dates_list[i]:
            qty_per_date = qty_per_date + data_frame_1.at[j, 'Order Quantity']
    order_dates_qty.append(qty_per_date)
df_e['Order Quantity'] = order_dates_qty

#print(df_e)
sheetn = 'e.'
worksheet = wkb.add_worksheet(sheetn)
df_e.to_excel(writer, sheetn, index = False)

#uncomment next 5 lines for e. plots
'''plt.plot(order_dates_list, order_dates_qty, color='green', marker='o', linestyle='solid')
plt.title("Orders per date")
plt.xlabel("Order Dates")
plt.ylabel("Quantity")
plt.show()'''


# f. -- total quantity harvested each day of the year --
df_f = pd.DataFrame()

harvest_dates = []
for i in data_frame_1.index:       #creates a list of harvest dates
    temp = data_frame_1.at[i, 'Harvest Date']
    if temp not in harvest_dates:
        harvest_dates.append(temp)
harvest_dates.sort()
df_f['Harvest Date'] = harvest_dates       # writes harvest dates list to df

harvest_date_qty = []
for i in harvest_dates:       # creates list of harvest quantity per date
    qty_per_harvest = 0
    for j in data_frame_1.index:
        if data_frame_1.at[j, 'Harvest Date'] == i:
            qty_per_harvest = qty_per_harvest + data_frame_1.at[j, 'Order Quantity']
    #print(i, qty_per_harvest)
    harvest_date_qty.append(qty_per_harvest)
df_f['Quantity'] = harvest_date_qty       # writes quantity of harvest to df

#print(df_f)
sheetn = 'f.'
worksheet = wkb.add_worksheet(sheetn)
df_f.to_excel(writer, sheetn, index = False)

#uncomment next 5 lines for f. plots
'''plt.plot(harvest_dates, harvest_date_qty, color='green', marker='o', linestyle='solid')
plt.title("Quantity Per Harvest Date")
plt.xlabel("Harvest Dates")
plt.ylabel("Quantity")
plt.show()'''


# g. -- total crop value distributed --
df_g = pd.DataFrame()

for i in data_frame_2.index:
    pounds_of_crop = 0
    crop_name = data_frame_2.at[i,'Crop Name']
    df_g.at[i, 'Crop Name'] = data_frame_2.at[i,'Crop Name']       # writes each crop name to df column
    for j in data_frame_1.index:
        if crop_name == data_frame_1.at[j, 'Product']:
            pounds_of_crop = pounds_of_crop + data_frame_1.at[j, 'Order Quantity']
    df_g.at[i, 'Dollars Sold'] = pounds_of_crop * data_frame_2.at[i, 'Value per Unit']       # writes lbs*value to df column

#print(df_g)
sheetn = 'g.'
worksheet = wkb.add_worksheet(sheetn)
df_g.to_excel(writer, sheetn, index = False)


# h. -- total crop servings distributed --
df_h = pd.DataFrame()

for i in data_frame_2.index:
    pounds_of_crop = 0
    crop_name = data_frame_2.at[i,'Crop Name']
    df_h.at[i, 'Crop Name'] = data_frame_2.at[i, 'Crop Name']       # writes crop name to df column
    for j in data_frame_1.index:
        if crop_name == data_frame_1.at[j, 'Product']:
            pounds_of_crop = pounds_of_crop + data_frame_1.at[j, 'Order Quantity']
    df_h.at[i, 'Servings Sold'] = pounds_of_crop * data_frame_2.at[i, 'Servings per Unit']       # writes lbs*servings per to df column

#print(df_h)
sheetn = 'h.'
worksheet = wkb.add_worksheet(sheetn)
df_h.to_excel(writer, sheetn, index = False)


# i. -- lbs yield per crop per square foot of field --
df_i = pd.DataFrame()

for i in data_frame_3.index:
    yield_per_lot = 0
    for j in data_frame_1.index:
        if data_frame_1.at[j, 'Lot code 1'] == data_frame_3.at[i, 'Lot code 1'] and data_frame_1.at[j, 'Lot code 2'] == data_frame_3.at[i, 'Lot code 2']:
            yield_per_lot = yield_per_lot + data_frame_1.at[j, 'Order Quantity']
    df_i.at[i, 'Lot Code'] = str(data_frame_3.at[i, 'Lot code 1']) + str(data_frame_3.at[i, 'Lot code 2'])       # writes lot codes to new df column
    df_i.at[i, 'Lbs. Yield Per Square Foot'] = (yield_per_lot / data_frame_3.at[i, 'Square feet'])       # writes yields per lot/sqft to new df column

#print(df_i)
sheetn = 'i.'
worksheet = wkb.add_worksheet(sheetn)
df_i.to_excel(writer, sheetn, index = False)


writer.save()       # save final xlsx file in specified location