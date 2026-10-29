local data = {}

data.k = 1.380649 * 10^-23 -- Дж/К
data.T = 300 -- К
data.e = 1.602176634 * 10^-19 -- Кл
data.epsilon_0 = 8.854187817 * 10^-14 -- Ф/см
data.epsilon_SI = 11.7 -- диэлектрическая проницаемость кремния

--- Задание 1

data.N_e = 10^20 -- см^-3
data.N_b = 10^18 -- см^-3
data.n_i = 1.45 * 10^10 -- см^-3

data.phi_k = (data.k * data.T / data.e) * math.log((data.N_e * data.N_b) / (data.n_i^2))
data.l_0 = math.sqrt((2 * data.epsilon_SI * data.epsilon_0 * data.phi_k / data.e) * (1 / data.N_e + 1 / data.N_b))
data.E_m_0 = 2 * data.phi_k / data.l_0

--- Задание 2

data.N_prime = 10^24 -- см^-4
data.phi_k_0_lin = 0.7 -- начальное приближение, В
data.l_0_0_lin = ((12 * data.epsilon_SI * data.epsilon_0 * data.phi_k_0_lin) / (data.e * data.N_prime))^(1/3)
data.phi_k_1_lin = 2 * (data.k * data.T / data.e) * math.log((data.N_prime * data.l_0_0_lin) / (2 * data.n_i))
data.l_0_1_lin = ((12 * data.epsilon_SI * data.epsilon_0 * data.phi_k_1_lin) / (data.e * data.N_prime))^(1/3)
data.phi_k_2_lin = 2 * (data.k * data.T / data.e) * math.log((data.N_prime * data.l_0_1_lin) / (2 * data.n_i))

local phi = data.phi_k_2_lin
local l0 = data.l_0_1_lin
for _ = 1, 20 do
  l0 = ((12 * data.epsilon_SI * data.epsilon_0 * phi) / (data.e * data.N_prime))^(1/3)
  phi = 2 * (data.k * data.T / data.e) * math.log((data.N_prime * l0) / (2 * data.n_i))
end
data.phi_k_lin = phi
data.l_0_lin = l0

--- Задание 3

data.S_1 = 10 -- см^2
data.C_0_abrupt = (data.epsilon_SI * data.epsilon_0 * data.S_1) / data.l_0 -- Ф
data.C_0_lin = (data.epsilon_SI * data.epsilon_0 * data.S_1) / data.l_0_lin -- Ф

--- Задание 4

data.U_1 = -5 -- В
data.U_2 = 0.5 -- В
data.C_abrupt_U1 = data.C_0_abrupt / math.sqrt(1 - data.U_1 / data.phi_k) -- Ф
data.C_abrupt_U2 = data.C_0_abrupt / math.sqrt(1 - data.U_2 / data.phi_k) -- Ф

--- Задание 5

data.C_lin_U1 = data.C_0_lin / ((1 - data.U_1 / data.phi_k_lin)^(1/3)) -- Ф
data.C_lin_U2 = data.C_0_lin / ((1 - data.U_2 / data.phi_k_lin)^(1/3)) -- Ф

return data
