import gym
import numpy as np
import pybullet as p
import pybullet_data

class QuadrupedEnv(gym.Env):
    def __init__(self):
        super().__init__()

        self.client = p.connect(p.GUI)
        p.setAdditionalSearchPath(pybullet_data.getDataPath())

        self.robot = None

        self.action_space = gym.spaces.Box(low=-1, high=1, shape=(8,))
        self.observation_space = gym.spaces.Box(low=-np.inf, high=np.inf, shape=(6,))

    def reset(self):
        p.resetSimulation()
        p.setGravity(0,0,-9.8)

        p.loadURDF("plane.urdf")
        self.robot = p.loadURDF("quadruped.urdf", [0,0,0.2])

        return self._get_obs()

    def step(self, action):
        for i in range(8):
            p.setJointMotorControl2(
                self.robot,
                i,
                p.POSITION_CONTROL,
                targetPosition=action[i]
            )

        p.stepSimulation()

        obs = self._get_obs()

        reward = self._compute_reward()

        done = False
        return obs, reward, done, {}

    def _get_obs(self):
        return np.random.randn(6)

    def _compute_reward(self):
        pos, _ = p.getBasePositionAndOrientation(self.robot)
        return pos[0]  # forward movement reward