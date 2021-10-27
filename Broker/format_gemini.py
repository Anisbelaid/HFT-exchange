import os
import csv

def SplitCsv ():
    with open('./csv_gemini.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            with open('ressouce/'+row['Account ID'], mode='a') as file:
                writer = csv.writer(file, delimiter=',')
                del row['Event Date']
                del row['Event Time']
                del row['Event Millis']
                del row['Gross Notional Value (USD)']
                del row['Fill Price (USD)']
                del row['Total Exec Quantity (BTC)']
                del row['Avg Price (USD)']
                del row['Fees (USD)']
                del row['Auction ID']
                del row['IOI ID']
                del row['']
                writer.writerow(row.keys())
                writer.writerow(row.values())
  

def replace():
    for filename in os.listdir('ressouce/'):
        if filename != 'clean':
            fin = open('ressouce/'+filename, "rt")
            data = fin.read()
            data = data.replace('Execution Options', 'TIF')
            data = data.replace('Fill Quantity (BTC)', 'QUANTITY')
            data = data.replace('Order ID', 'CLIENTID')
            data = data.replace('Account ID', 'ACCOUNT')
            data = data.replace('Limit Price (USD)', 'PRICE')
            data = data.replace('Event Type', 'TYPE')
            data = data.replace('Remaining Quantity (BTC)', 'REMQUANTITY')
            data = data.replace('Order Type', 'ORDERTYPE')
            fin.close()
            fin = open('ressouce/clean/'+filename, "wt")
            fin.write(data)
            fin.close()
            
def removeDuplicate():
    for filename in os.listdir('ressouce/'):
        if (filename != 'clean'):
            lines_seen = set() # holds lines already seen
            outfile = open('ressouce/clean/'+filename+'_clean', "w")
            for line in open('ressouce/'+filename, "r"):
                if line not in lines_seen: # not a duplicate
                    outfile.write(line)
                    lines_seen.add(line)
            outfile.close()
        
def main():
    #SplitCsv()
    replace()
    #removeDuplicate()
  
if __name__== "__main__":
  main()
