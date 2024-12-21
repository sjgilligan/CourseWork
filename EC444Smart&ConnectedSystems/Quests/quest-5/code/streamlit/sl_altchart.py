import streamlit as st
import requests
import pandas as pd
import numpy as np
import datetime 
import time 
import altair as alt

# Function to fetch data from the Express.js API
def fetch_data_from_api():
    # Change the URL to your actual Express API endpoint
    url = 'http://localhost:3000/api/data'
    try:
        response = requests.get(url)
        response.raise_for_status()  # Check if request was successful
        data = response.json() # Convert response to JSON
        return pd.DataFrame(data); #convert json to data frame 
    except requests.exceptions.RequestException as e:
        st.error(f"Error fetching data: {e}")
        return pd.DataFrame(); #return empty data frame on error 

# Streamlit app
def main():
    st.title("Live Graph of Car")
    now = datetime.datetime.now()
    formatted_time = now.strftime("%H:%M") #used google ai 
    st.write("Current Time:", formatted_time)

    checkpoints = pd.DataFrame({ 
         "X" : [-725,745, -728, 765],
         "Z" : [-455, -467, 730, 748],
         "Type" : ["Checkpoint"] *4 
    })
    chart_placeholder = st.empty()
    
     # Initialize an empty DataFrame
    live_data = pd.DataFrame()

    # Refresh interval (seconds)
    refresh_interval = 2

    # Run the live chart loop
    while True:
        # Fetch data from the API
        new_data = fetch_data_from_api()
        #print(new_data["X"]) 
        #print(new_data["Z"])
        #st.write(new_data)
        if not new_data.empty:
            # Concatenate new data with the existing DataFrame
                live_data = pd.concat([live_data, new_data], ignore_index=True)

            # Add labels for live data based on ID
                live_data["Type"] = live_data["id"].replace({7: "ID 7", 8: "ID 8"})

            # Merge live data with checkpoints
                plot_data = pd.concat([
                    live_data[["X", "Z", "Type"]],
                    checkpoints
                ], ignore_index=True)

            # Create Altair scatter plot with custom colors for each type
                scatter_plot = alt.Chart(plot_data).mark_line(point=True).encode(
                    x="X:Q",
                    y="Z:Q",
                    color=alt.Color("Type:N", scale=alt.Scale(domain=["ID 7", "ID 8", "Checkpoint"],
                                                            range=["blue", "green", "red"])),
                    tooltip=["X", "Z", "Type"]
                ).properties(
                    width=700,
                    height=500,
                    title="Scatter Plot with IDs and Checkpoints"
                )
                with chart_placeholder.container():
                    st.altair_chart(scatter_plot, use_container_width=True) # plot the scatter plot
        else:
                    st.warning("No new data fetched. Retrying...")

        # Wait before fetching new data
        time.sleep(refresh_interval) 

if __name__ == "__main__":
    main()