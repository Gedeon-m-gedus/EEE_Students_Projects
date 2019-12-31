import thingspeak
import json
import time
import matplotlib.pyplot as plt





def plot_my_data():
    x=0
    xx=0
    
    temp_list = []
    turb_list = []
    x_cordinates = []
    while x>=0 and xx==0:
        print("good")
        x_cordinates.append(x)
        x = x + 1
        ch = thingspeak.Channel(id=817351, api_key='H8QFEOWP3N5FFHC6',fmt='json')
        data_from_thingspeak=ch.get({'results': 2})


        data_list_from_thingspeak=data_from_thingspeak.split(",")

        #print(data_from_thingspeak)
        #print(data_list_from_thingspeak)

        print("")
        print("")
        temperature=data_list_from_thingspeak[12]
        temperature=temperature.split('"')
        temperature = temperature[3]
        print("Temperature = ",temperature)
        temp_list.append(float(temperature))

        print("")
        print("")

        #time.sleep(1)

        turbidity=data_list_from_thingspeak[13]
        turbidity=turbidity.split('"')
        turbidity=turbidity[3]
        turbidity=turbidity.split(" ")
        turbidity = turbidity[0]
        print("Turbidity = ",turbidity)
        turb_list.append(float(turbidity))

        #time.sleep(1)

        #Ploting data

        #plt.scatter(x_cordinates,temp_list,color='r')
        plt.plot(x_cordinates,temp_list,color='r')
        plt.plot(x_cordinates,turb_list,color='b')
        #labeling x and y axis
        plt.xlabel('Data intries No')
        plt.ylabel('Data values')

        #titlle of my plot
        plt.title('Water Quality Monitoring System')
        #to show the graph
        #plt.show()
        plt.pause(0.0001)
        print("good_good")
        time.sleep(12)

        
        if x==100:
            xx=5
    print(x_cordinates)
    print(temp_list)
    print(turb_list)



