from stable_baselines3 import PPO
from env import QuadrupedEnv

env = QuadrupedEnv()
model = PPO("MlpPolicy", env, verbose=1)
model.learn(total_timesteps=200000)
model.save("quadruped_model")
