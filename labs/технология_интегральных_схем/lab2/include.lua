local include = {}

---@diagnostic disable: lowercase-global
---@diagnostic disable-next-line: undefined-field
local tex = _G.tex

function include.value_with_mantice(value, precision, shift, scale)
  local function round_fallback(x)
    if x >= 0 then return math.floor(x + 0.5) else return math.ceil(x - 0.5) end
  end

  precision = precision or 5
  shift = shift or 0
  scale = scale or 0

  if value == nil or value ~= value then
    return "\\fpeval{0}"
  end
  if value == 0 then
    return "\\fpeval{0}"
  end

  local sign = ""
  if value < 0 then
    sign = "-"
    value = math.abs(value)
  end

  local logv = math.log(value, 10)
  if not (logv == logv) then -- проверка на NaN
    return "\\fpeval{0}"
  end

  local floorlog = math.floor(logv)
  local mantissa = round_fallback(value * 10^(precision - 1 - floorlog)) * 10^(-(precision - 1) + shift - scale)
  local exponent = floorlog - shift

  if exponent == 0 then
    return sign .. "\\fpeval{" .. mantissa .. "}"
  else
    return sign .. "\\fpeval{" .. mantissa .. "} \\cdot 10^{" .. exponent .. "}"
  end
end

function include.pgf_plot_wrap(title, 
                               xlabel, ylabel, 
                               multiplicator_x, multiplicator_y,
                               ...)
  local args = {...}

  local function compare_func(array, literal, func)
    local val = 0
    local index = 0
    if literal == "x" then
        index = 0
    elseif literal == "y" then
        index = 1
    else
        index = 2
    end
    val = array[1][5 + index][1]
    for i = 1, #array do
      local data = array[i][5 + index]
      for j = 2, #data do
        val = func(val, data[j])
      end
    end
    return val
  end

  local function min(array, literal)
    return compare_func(array, literal, math.min)
  end

  local function max(array, literal)
    return compare_func(array, literal, math.max)
  end

  tex.sprint("\\begin{tikzpicture}")
  tex.sprint("\\begin{axis}[")
  tex.sprint("    title={" .. title .. "}, ")
  tex.sprint("    xlabel={" .. xlabel .. "}, ")
  tex.sprint("    ylabel={" .. ylabel .. "}, ")
  tex.sprint("    width=" .. multiplicator_x .. "\\textwidth, ")
  tex.sprint("    height=" .. multiplicator_y .. "\\textwidth, ")
  tex.sprint("    grid=both, ")
  tex.sprint("    grid style={dashed, gray!30}, ")
  tex.sprint("    xmin=" .. -0.1 * max(args, "x") + min(args, "x") .. ", xmax=" .. 1.1 * max(args, "x") .. ", ")
  tex.sprint("    ymin=" .. -0.1 * max(args, "y") + min(args, "y") .. ", ymax=" .. 1.1 * max(args, "y") .. ", ")
  tex.sprint("    legend pos=north west ")
  tex.sprint("]")

  for i = 1, #args do
    local data_x = args[i][5]
    local data_y = args[i][6]
    tex.sprint("\\addplot[")
    tex.sprint("    color=" .. (args[i][1] or "blue") .. ", ")
    tex.sprint("    mark=" .. (args[i][2] or "*") .. ", ")
    tex.sprint("    mark size=" .. (args[i][3] or "2pt") .. ", ")
    tex.sprint("    thick ")
    tex.sprint("] coordinates {")
    for j = 1, #data_x do
      tex.sprint("(" .. data_x[j] .. "," .. data_y[j] .. ")")
    end
    tex.sprint("};")
    if args[i][4] then
      tex.sprint("\\addlegendentry{" .. args[i][4] .. "}")
    end
  end

  tex.sprint("\\end{axis}")
  tex.sprint("\\end{tikzpicture}")
end

return include