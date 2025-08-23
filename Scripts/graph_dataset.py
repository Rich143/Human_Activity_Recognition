import pandas as pd
import plotly.graph_objs as go
from plotly.subplots import make_subplots
import sys

if len(sys.argv) < 2:
    print("Usage: python graph_dataset.py <dataset_csv>")
    sys.exit(1)

csv_file = sys.argv[1]

# Load CSV with named columns
columns = [
    "unproc_x", "unproc_y", "unproc_z",
    "lpf_x", "lpf_y", "lpf_z",
    "proc_x", "proc_y", "proc_z",
    "model_output_0","model_output_1","model_output_2","model_output_3",
    "output_class","contains_output"
]

df = pd.read_csv(csv_file, names=columns, skiprows=1)

df["time"] = df.index * 0.01  # 100 Hz sampling

# Helper to plot one signal type (e.g., unproc, lpf, proc)
def plot_signal_set(df, prefix, title):
    fig = make_subplots(rows=3, cols=1, shared_xaxes=True, shared_yaxes=True,
                        subplot_titles=(f"{title} X", f"{title} Y", f"{title} Z"),
                        vertical_spacing=0.03)

    fig.add_trace(go.Scatter(x=df["time"], y=df[f"{prefix}_x"], name="X"), row=1, col=1)
    fig.add_trace(go.Scatter(x=df["time"], y=df[f"{prefix}_y"], name="Y", marker_color="orange"), row=2, col=1)
    fig.add_trace(go.Scatter(x=df["time"], y=df[f"{prefix}_z"], name="Z", marker_color="green"), row=3, col=1)

    # Optional: sync y-axes
    y_all = pd.concat([df[f"{prefix}_x"], df[f"{prefix}_y"], df[f"{prefix}_z"]])
    y_min, y_max = y_all.min(), y_all.max()
    for i in range(1, 4):
        fig.update_yaxes(range=[y_min, y_max], row=i, col=1)

    fig.update_layout(height=600, width=900,
                      title_text=f"{title} Acceleration Data (100 Hz)",
                      xaxis3_title="Time (s)",
                      showlegend=False)

    fig.show()

# Plot each signal group
plot_signal_set(df, "unproc", "Unprocessed")
# plot_signal_set(df, "lpf", "Low-pass Filtered")
# plot_signal_set(df, "proc", "Processed")
