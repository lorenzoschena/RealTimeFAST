import zmq 
import multiprocessing as mp 
import numpy as np 
import plotly.graph_objs as go
from plotly.subplots import make_subplots
import plotly.io as pio
import threading 
from IPython.display import display, clear_output
import time 

class RFInteractor: 
    ""
    
    ""
    def __init__(self, 
                 ZmqInAddress: str = 'tcp://127.0.0.1:5555', 
                 ZmqOutAddress: str = 'tcp://127.0.0.1:5556', 
                 live_plot: bool = True, 
                 verbose: bool = False): 
        
        self.ZmqInAddress = ZmqInAddress
        self.ZmqOutAddress = ZmqOutAddress
        self.live_plot = live_plot
        self.plot_lock = threading.Lock()
        self.fig = False
        self.allowable_requests = ['VelH', 'VelV', 'AngleH', 'AngleV', 'BlPitchCom1', 
                                   'BlPitchCom2', 'BlPitchCom3', 'GenTq', 'alpha']
        self.verbose = verbose
        
        # ------ Connect to Publisher
        if self.ZmqOutAddress is not None: 
            self.pub_context = zmq.Context()
            self.subscriber = self.pub_context.socket(zmq.SUB)
            self.subscriber.bind(self.ZmqOutAddress)
            self.subscriber.setsockopt_string(zmq.SUBSCRIBE, "") # for now subscribe to all

            self.sub_dict = {}
            
        print('ZMQ Real Time interactor for FAST initialized. \n PUB-SUB protocol: {} | REQ-REP protocol: {}'.format(self.ZmqOutAddress, 
                                                                                                                     self.ZmqInAddress))
        pass 
    
    
    @staticmethod
    def _update_dict(update_, dict_):
        for key, value in update_.items():
            if key not in dict_:
                dict_[key] = []  
                dict_[key].append(value)
            else: 
                dict_[key].append(value)
                
        return dict_
    
    def update_plot(self):
        if not self.fig:
            # Exclude 'Time' and 'TurbId' from subplot titles
            subplot_keys = [key for key in self.sub_dict.keys() if key not in [' Time', 'TurbId']]
            # Initialize subplots only if they haven't been initialized yet
            self.fig = make_subplots(rows=len(subplot_keys), cols=1, subplot_titles=subplot_keys)
            
            row_index = 1  # Initialize row index
            
            for trace_name in subplot_keys:
                self.fig.add_scatter(y=self.sub_dict[trace_name], mode='lines', name=trace_name, row=row_index, col=1)
                row_index += 1  # Increment row index
            
            self.fig.update_layout(height=1200, title_text=f"TurbId: {self.sub_dict['TurbId'][-1]} - Time = {self.sub_dict[' Time'][-1]} (s)")  
            display(self.fig)  # Display the initial figure
        
        else:
            subplot_keys = [key for key in self.sub_dict.keys() if key not in [' Time', 'TurbId']]
            for i, trace_name in enumerate(subplot_keys):
                self.fig.data[i].y = self.sub_dict[trace_name]
                    
            self.fig.update_layout(height=1200, title_text=f"TurbId: {self.sub_dict['TurbId'][-1]} - Time = {self.sub_dict[' Time'][-1]} (s)")  
            clear_output(wait=True)
            display(self.fig)



    
    def fast_sub(self, N_plots_update = 100):
        """
        Dummy function for the subscription to the FAST channel
        TODO: Need to split acquisition and plotting, maybe multiprocessing, to avoid messages loss
        """
        count = 0
        
        while True: 
            update_ = self.subscriber.recv_json()
                        
            if self.verbose:
                print(update_)

            self.sub_dict = self._update_dict(update_, self.sub_dict)
            
            if count == 0: 
                self.data_length = len(self.sub_dict)

            count += 1
            if count % N_plots_update == 0:
                self.update_plot()
                # time.sleep(1)
        
                    



