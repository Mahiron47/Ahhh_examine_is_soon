local handle = {}

local delta_n = {}

delta_n.__index = delta_n

function delta_n:new(delta_n0, tau_n)
    local instance = setmetatable({}, self)
    instance.delta_n0 = delta_n0
    instance.tau_n = tau_n
    return instance
end

function delta_n:evaluate(t)
    return self.delta_n0 * math.exp(-t / self.tau_n)
end

function delta_n:get_values(t_list)
    local values = {}
    for _, t in ipairs(t_list) do
        table.insert(values, self:evaluate(t))
    end
    return values
end

handle.delta_n = delta_n

return handle