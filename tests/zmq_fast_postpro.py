from pyFAST.input_output import FASTOutputFile
import numpy as np 
import matplotlib.pyplot as plt 

df_fast = FASTOutputFile('./templateDir/Main_tsim.outb').toDataFrame()
ts_out = FASTOutputFile('./tsim_rpc_1.dat').toDataFrame()

VelXFast = df_fast['Wind1VelX_[m/s]']

print(len(VelXFast))

VelXTSim = ts_out['U_[3.40]']

sent_uh = np.load('./sent_uh.npz')['arr_0']
t = np.linspace(0, 10, len(VelXFast))
# tt = np.linspace(0, 10, len(VelXTSim))
############
fig, ax = plt.subplots(2, 1, figsize=(7, 9), sharex=True)

ax[0].plot(VelXFast, label='Wind1VelX_(m/s)_FAST')
ax[0].plot(sent_uh, label='Wind1VelX_(m/s)_TSim', ls='--')
ax[0].set(ylabel='$u_\infty$ (m/s)', ylim=(9.5, 15.5))
ax[0].legend()

ax[1].plot(np.abs(VelXFast[1:] - sent_uh)/sent_uh)
ax[1].set(yscale='log', ylabel='$\epsilon_r$ (-)', xlabel='dt')

fig.tight_layout()

plt.savefig('./t_10_comm_check.png')
plt.close()
############
fig, ax = plt.subplots(figsize=(4, 4))
ax.scatter(VelXFast[1:], sent_uh, alpha=0.65, color='blue')
ax.set(xlabel='VelXFast', ylabel='VelXTSim', aspect='equal', ylim=(9, 15.5),
       xlim=(9, 15.5))

ax.plot(VelXFast[1:], VelXFast[1:], ls='--')
ax.grid()

fig.tight_layout()
plt.savefig('./t_10_comm_scatter.png')
plt.close()
#########
sent_signal = (sent_uh - np.mean(sent_uh)) / np.std(sent_uh)
received_signal = (VelXFast[1:] - np.mean(VelXFast[1:])) / np.std(VelXFast[1:])

cross_corr = np.correlate(received_signal, sent_signal, mode='full')
lags = np.arange(-len(sent_uh) + 1, len(sent_uh))

# Plotting the cross-correlation
plt.figure(figsize=(8, 4))
plt.stem(lags, cross_corr)
plt.xlabel('Lag')
plt.ylabel('Cross-correlation')
plt.title('Cross-correlation between Sent and Received Signals')
plt.grid(True)
plt.savefig('./cross_corr.png')