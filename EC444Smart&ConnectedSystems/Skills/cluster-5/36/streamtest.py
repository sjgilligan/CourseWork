#Sebastian Gilligan, run using 'streamlit run streamtest.py', 
#made with chatgpt 
import streamlit as st
from tinydb import TinyDB, Query
import matplotlib.pyplot as plt
import numpy as np

# Initialize the database
db = TinyDB('database.json')

# Streamlit App
st.title("2D Grid Plot with Angles and TinyDB")

# Add a new entry
st.subheader("Add a new entry")
entry_id = st.text_input("ID")
x_value = st.number_input("X Value", step=1.0)
z_value = st.number_input("Z Value", step=1.0)
angle = st.number_input("Angle (degrees)", step=1.0, min_value=0.0, max_value=360.0)
status = st.selectbox("Status", ["Active", "Inactive"])

if st.button("Add to Database"):
    if entry_id and status:
        db.insert({
            'id': entry_id,
            'x': x_value,
            'z': z_value,
            'angle': angle,
            'status': status
        })
        st.success(f"Added entry ID: {entry_id} to the database.")
    else:
        st.error("Please provide an ID and a status.")

# Display the database
st.subheader("Database Contents")
data = db.all()
if data:
    st.write(data)
else:
    st.write("No entries in the database.")

# Search the database
st.subheader("Search the database")
search_id = st.text_input("Search by ID")
if st.button("Search"):
    if search_id:
        Entry = Query()
        results = db.search(Entry.id == search_id)
        if results:
            st.write(f"Found: {results}")
        else:
            st.warning("No matching entries found.")
    else:
        st.error("Please enter an ID to search.")

# Delete an entry
st.subheader("Delete an entry")
delete_id = st.text_input("Delete by ID")
if st.button("Delete"):
    if delete_id:
        Entry = Query()
        deleted_count = db.remove(Entry.id == delete_id)
        if deleted_count:
            st.success(f"Deleted all entries for ID: {delete_id}.")
        else:
            st.warning("No matching entries found to delete.")
    else:
        st.error("Please enter an ID to delete.")

# Plot the 2D Grid with Angles
st.subheader("2D Grid Plot with Angles")
if st.button("Plot Data"):
    if data:
        x_values = [entry['x'] for entry in data]
        z_values = [entry['z'] for entry in data]
        angles = [entry['angle'] for entry in data]
        statuses = [entry['status'] for entry in data]
        ids = [entry['id'] for entry in data]

        # Map statuses to colors
        colors = ['green' if status == 'Active' else 'red' for status in statuses]

        # Create the plot
        fig, ax = plt.subplots(figsize=(8, 8))
        ax.scatter(x_values, z_values, c=colors, alpha=0.8, edgecolor="k")

        # Add arrows for angles
        for x, z, angle, id_ in zip(x_values, z_values, angles, ids):
            dx = np.cos(np.radians(angle))  # X component of the arrow
            dz = np.sin(np.radians(angle))  # Z component of the arrow
            ax.arrow(x, z, dx * 0.5, dz * 0.5, head_width=0.2, head_length=0.2, fc='blue', ec='blue')
            ax.text(x, z, f'{id_}', fontsize=9, ha='right')

        ax.set_title("2D Grid Plot with Angles")
        ax.set_xlabel("X Value")
        ax.set_ylabel("Z Value")
        ax.axhline(0, color='black', linewidth=0.5, linestyle="--")
        ax.axvline(0, color='black', linewidth=0.5, linestyle="--")
        st.pyplot(fig)
    else:
        st.warning("No data available to plot.")

# Clear the database
if st.button("Clear Database"):
    db.truncate()
    st.warning("Database cleared!")
