local data = {}

data.k = 1.380649 * 10^-23 -- Дж/К
data.T = 300 -- К
data.e = 1.602176634 * 10^-19 -- Кл
data.phi_T = data.k * data.T / data.e -- В
data.n_i = 1.45 * 10^10 -- см^-3
data.epsilon_0 = 8.854187817 * 10^-14 -- Ф/см
data.epsilon_SI = 11.7

--- Задание 1
data.N_e = 10^20 -- см^-3 (эмиттер n+)
data.N_b = 10^18 -- см^-3 (база p)

-- Встроенные потенциалы относительно собственного полупроводника (уровня Ферми i-Si)
data.phi_0e = data.phi_T * math.log(data.N_e / data.n_i) -- В
data.phi_0b = data.phi_T * math.log(data.N_b / data.n_i) -- В
data.phi_k = data.phi_0e + data.phi_0b -- Полная контактная разность потенциалов, В

-- Падения контактного потенциала на ОПЗ эмиттера и базы
data.delta_phi_0e = data.phi_k * (data.N_b / (data.N_e + data.N_b)) -- В
data.delta_phi_0b = data.phi_k * (data.N_e / (data.N_e + data.N_b)) -- В

--- Задание 2
data.trans_type = "n^+-p"

--- Задание 3
data.delta_inj = 1
data.n_p0 = (data.n_i^2) / data.N_b -- см^-3
data.U_delta_1 = data.phi_T * math.log(1 + (data.N_b^2) / (data.n_i^2)) -- В

--- Задание 4
data.region_type = "n"
data.w_um = 1 -- мкм
data.w = data.w_um * 10^-4 -- см
data.N_d = 10^18 -- см^-3
data.tau_ns = 10 -- нс
data.tau = data.tau_ns * 10^-9 -- с
data.p_n0 = (data.n_i^2) / data.N_d -- см^-3
data.mu_p = 150 -- см^2/(В·с) (подвижность дырок в Si при N = 10^18 см^-3)
data.D_p = data.phi_T * data.mu_p -- см^2/с
data.L_p = math.sqrt(data.D_p * data.tau) -- см
data.L_p_um = data.L_p * 10^4 -- мкм

-- Концентрации при U = +5 phi_T
data.U_fwd = 5 * data.phi_T
data.p_0_fwd = data.p_n0 * math.exp(5)
data.delta_p_0_fwd = data.p_n0 * (math.exp(5) - 1)

-- Концентрации при U = -5 phi_T
data.U_rev = -5 * data.phi_T
data.p_0_rev = data.p_n0 * math.exp(-5)
data.delta_p_0_rev = data.p_n0 * (math.exp(-5) - 1)

return data
