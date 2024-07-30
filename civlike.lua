local function check_rules(rules)

   local function check_type(field, tp, mandatory, idx)

      mandatory = mandatory or false

      local value, field_msg
      if type(field) == "string" then
         value = rules[field]
         field_msg = "Field '" .. field .. "' "
      elseif type(field) == "table" then
         field_msg = "Field '" .. table.concat(field, '.') .. "' "
         value = rules
         for _,f in ipairs(field) do
            value = value[f]
         end
      end

      if value == nil then
         if not mandatory then return end
         if mandatory then error(field_msg .. "is mandatory.") end
      end

      if type(tp) == "string" then
         if tp == "point" then
            if type(value) ~= "table" or #value ~= 2 or type(value[1]) ~= "number" or type(value[2]) ~= "number" then
               error(field_msg .. "should be a point (like {0, 0}).")
            end
         elseif tp == "id" then
            if type(value) ~= "string" or #value ~= 1 then
               error(field_msg .. "should be an id (single char).")
            end
         elseif tp == "color" then
            if type(value) ~= "string" or value:match("^#?%x%x%x%x%x%x$") == nil then
               error(field_msg .. "should be an color (#abcdef).")
            end
         end
      end
   end

   local function check_array(field, subfield, tp, mandatory)
      if type(rules[field]) ~= "table" then error("Field '" .. field .. "' should be a table.") end
      for i = 1,#rules[field] do
         check_type({ field, i, subfield }, tp, mandatory)
      end
   end

   check_type("map_size", "point", true)
   check_array("terrains", "id", "id", true)
   check_array("terrains", "color", "color", true)
end

return {

   create_game = function(rules)

      check_rules(rules)

      local game = {
         map = {},
         nations = {},
         units = {},
      }

      for y=1,rules.map_size[2] do

      end

      return game
   end,

}

-- vim:ts=3:sts=3:sw=3:expandtab:nowrap
