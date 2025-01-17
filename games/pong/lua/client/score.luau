function generateScore()
    local score = {}
    score[1] = 0
    score[2] = 0
    return score
end

function drawScore(score, player)
    if player == 1 then
        score_text = root:CreateChild("Label", "score_text", 800/2 - 70, 10, score[1], "assets/bit5x3.ttf", 50)
        return score_text
    else
        score_text_player2 = root:CreateChild("Label", "score_text_player2", 800/2 + 30, 10, score[2], "assets/bit5x3.ttf", 50)
        return score_text2
    end
end

function updateScore(score, player, score_text)
    score[player] = score[player] + 1
    score_text:SetText(score[player])
end

function resetScore(score, score_text, score_text_player2)
    score[1] = 0
    score[2] = 0
    score_text:SetText(score[1])
    score_text_player2:SetText(score[2])
end
