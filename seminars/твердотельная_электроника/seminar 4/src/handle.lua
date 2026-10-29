local handle = {}

local minority_dist = {}
minority_dist.__index = minority_dist

function minority_dist:new(pn0, U_ratio, w, Lp)
    local instance = setmetatable({}, self)
    instance.pn0 = pn0
    instance.U_ratio = U_ratio
    instance.w = w
    instance.Lp = Lp
    return instance
end

function minority_dist:evaluate(x)
    local delta_p0 = self.pn0 * (math.exp(self.U_ratio) - 1)
    local delta_p = delta_p0 * math.sinh((self.w - x) / self.Lp) / math.sinh(self.w / self.Lp)
    return self.pn0 + delta_p
end

function minority_dist:get_values(x_list)
    local values = {}
    for _, x in ipairs(x_list) do
        table.insert(values, self:evaluate(x))
    end
    return values
end

handle.minority_dist = minority_dist

return handle
