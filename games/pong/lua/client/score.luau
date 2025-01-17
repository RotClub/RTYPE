function generateScore()
    local score = {}
    score[1] = 0
    score[2] = 0
    return score
end

function drawScore(score)
    score_text = root:CreateChild("Label", "score_text", 10, 10, "Player 1: " .. score[1] .. " Player 2: " .. score[2], "assets/bit5x3.ttf", 30)
    return score_text
end

function updateScore(score_text, score, player)
    score[player] = score[player] + 1
    score_text:SetText("Player 1: " .. score[1] .. " Player 2: " .. score[2])
end

function resetScore(score_text, score)
    score[1] = 0
    score[2] = 0
    score_text:SetText("Player 1: " .. score[1] .. " Player 2: " .. score[2])
end
