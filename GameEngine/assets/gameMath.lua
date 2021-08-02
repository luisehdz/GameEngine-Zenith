--returns the length of a line from the origin to x, y.
function Magnitude(x, y)
    return math.sqrt(x * x + y * y)
end

--returns the distance between two 2D points
function Distance(x1, y1, x2, y2)
    return Magnitude(x2 - x1, y2 - y1)
end

--returns true if two circles overlap
function CirclesOverlap(x1, y1, radius1, x2, y2, radius2)
    return Distance(x1, y1, x2, y2) <= radius1 + radius2
end

--returns true if two boxes overlap
function BoxesOverlap(x1, y1, w1, h1, x2, y2, w2, h2)
	if x1 >= x2 + w2 then return false end
	if y1 >= y2 + h2 then return false end
	if x2 >= x1 + w1 then return false end
	if y2 >= y1 + h1 then return false end
    
    return true
end

--circle has x, y, radius; box has x, y, w, h.
--returns true if the circle and box are overlapping.
function CircleBoxOverlap(circle, box)
    local closestX;
    local closestY;
    
    if circle.x < box.x then
        closestX = box.x
    elseif circle.x > box.x + box.w then
        closestX = box.x + box.w
    else
        closestX = circle.x
    end
    
    if circle.y < box.y then
        closestY = box.y
    elseif circle.y > box.y + box.h then
        closestY = box.y + box.h
    else
        closestY = circle.y
    end
    
    return Distance(closestX, closestY, circle.x, circle.y) < circle.radius;
end
