function generateScore()
    local score = {}
    score[1] = 0
    score[2] = 0
    return score
end

function generateWinnerText()
    local winner_text = root:CreateChild("Label", "winner_text", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 50, "", "assets/bit5x3.ttf", 50)
    return winner_text
end

function drawScore(score, player)
    if player == 1 then
        local score_text = root:CreateChild("Label", "score_text", WINDOW_WIDTH/2 - 40, 10, score[1], "assets/bit5x3.ttf", 50)
        return score_text
    elseif player == 2 then
        local score_text_player2 = root:CreateChild("Label", "score_text_player2", WINDOW_WIDTH/2 + 25, 10, score[2], "assets/bit5x3.ttf", 50)
        return score_text_player2
    end
end

function updateScore(score, score_text, score_text_player2)
    if score[1] > 0 then
        score_text:SetText(score[1])
    end
    if score[2] > 0 then
        score_text_player2:SetText(score[2])
    end
end

function resetScore(score, score_text, score_text_player2)
    score[1] = 0
    score[2] = 0
    score_text:SetText(score[1])
    score_text_player2:SetText(score[2])
end

function checkWinner(score, score_text, score_text_player2)
    if score[1] >= 7 then
        resetScore(score, score_text, score_text_player2)
        return 1
    elseif score[2] >= 7 then
        resetScore(score, score_text, score_text_player2)
        return 2
    else
        return 0
    end
end
