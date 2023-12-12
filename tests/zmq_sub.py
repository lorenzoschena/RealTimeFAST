import zmq
import pickle
import numpy as np 
import plotly.graph_objs as go
import plotly.subplots as sp
# ----------------------------------------- # 
def pub_sub_handler():
    context = zmq.Context()
    subscriber = context.socket(zmq.SUB)
    subscriber.bind("tcp://127.0.0.1:5556")
    subscriber.setsockopt_string(zmq.SUBSCRIBE, "") 

    
    fig = sp.make_subplots(rows=3, cols=1, subplot_titles=('WindVelX', 'WindVelY', 'WindVelZ'))
    fig.add_scatter(y=[], mode='lines', name='WindVelX')
    fig.add_scatter(y=[], mode='lines', name='WindVelY')
    fig.add_scatter(y=[], mode='lines', name='WindVelZ')

    received_updates = []
    print('Starting pubsub...\n')

    # try:
    while True:
        update = subscriber.recv_string()
        print("Received message:", update)
        received_updates.append(update)

        # data = {}
        # lines = update.split('\n')
        # for line in lines:
        #     key_val = line.split('=')
        #     if len(key_val) == 2:
        #         key = key_val[0].strip()
        #         val = float(key_val[1].strip())
        #         data[key] = val

        # # Update the plot based on the received data
        # if 'time' in data:
        #     timestamp = data['time']
        #     if 'WindVelX (m/s)' in data:
        #         fig['data'][0]['y'] += [data['WindVelX']]
        #         fig['data'][0]['x'] += [timestamp]
        #     if 'WindVelY (m/s)' in data:
        #         fig['data'][1]['y'] += [data['WindVelY']]
        #         fig['data'][1]['x'] += [timestamp]
        #     if 'WindVelZ (m/s)' in data:
        #         fig['data'][2]['y'] += [data['WindVelZ']]
        #         fig['data'][2]['x'] += [timestamp]

        #     # Update layout and save the figure as an HTML file
        #     fig.update_layout(title='Real-time Data Plot', xaxis_title='Time', yaxis_title='Velocity')
        #     fig.write_html("real_time_plot.html")  # Save the figure as an HTML file


if __name__ == '__main__':
    pub_sub_handler()