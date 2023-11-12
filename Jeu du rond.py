import pygame
import sys

# Initialisation de Pygame
pygame.init()

# Paramètres de la fenêtre
width, height = 1000, 750
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Jeu du rond")

# Couleurs
black = (0, 0, 0)
red = (255, 0, 0)
blue = (0, 0, 255)

# Paramètres du cercle
circle_radius = 30
circle_thickness = 5
circle_center = (width // 2, height // 2)

# Paramètres du rond mobile
moving_circle_radius = 20
moving_circle_speed = 10
moving_circle_direction = 1  # 1 pour droite, -1 pour gauche
moving_circle_rect = pygame.Rect(width - circle_radius - moving_circle_radius, circle_center[1] - moving_circle_radius,
                                  moving_circle_radius * 2, moving_circle_radius * 2)

# Paramètres du jeu
game_active = False
current_score = 0
best_score = 0
pause_count = 0
perfect_score_count = 0

# Boucle principale du jeu
clock = pygame.time.Clock()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                if game_active:
                    # Si le jeu était actif, on le met en pause et enregistre le score
                    game_active = False
                    best_score = max(best_score, current_score)
                    pause_count += 1
                    if current_score == 100:
                        perfect_score_count += 1
                else:
                    # Si le jeu était en pause, on le redémarre
                    game_active = True
                    current_score = 0
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Clic gauche de la souris
                if game_active:
                    # Si le jeu était actif, on le met en pause et enregistre le score
                    game_active = False
                    best_score = max(best_score, current_score)
                    pause_count += 1
                    if current_score == 100:
                        perfect_score_count += 1
                else:
                    # Si le jeu était en pause, on le redémarre
                    game_active = True
                    current_score = 0

    screen.fill(black)

    # Dessiner le cercle
    pygame.draw.circle(screen, black, circle_center, circle_radius)
    pygame.draw.circle(screen, red, circle_center, circle_radius - circle_thickness)

    # Déplacer le rond en va-et-vient sur toute la largeur de l'écran si le jeu est actif
    if game_active:
        moving_circle_rect.x += moving_circle_speed * moving_circle_direction

        # Inverser la direction lorsque le cercle atteint les bords de l'écran
        if moving_circle_rect.right > width or moving_circle_rect.left < 0:
            moving_circle_direction *= -1

        # Calculer le pourcentage de réussite
        distance = abs(moving_circle_rect.centerx - circle_center[0])
        if distance > circle_radius - circle_thickness + moving_circle_radius:
            current_score = 0
        else:
            current_score = min(100, 100 - ((distance - (circle_radius - circle_thickness - moving_circle_radius)) /
                                           (2 * moving_circle_radius) * 100))
            current_score = max(current_score, 0)

    # Dessiner le rond mobile en bleu
    pygame.draw.circle(screen, blue, moving_circle_rect.center, moving_circle_radius)

    # Afficher les scores avec deux chiffres après la virgule
    font = pygame.font.Font(None, 36)
    current_score_text = font.render(f"Score : {current_score:.2f}%", True, red)
    best_score_text = font.render(f"Meilleur score : {best_score:.2f}%", True, red)
    pause_count_text = font.render(f"Nombre d'éssais : {pause_count}", True, red)
    perfect_score_count_text = font.render(f"Scores parfaits : {perfect_score_count}", True, red)
    screen.blit(current_score_text, (10, 10))
    screen.blit(best_score_text, (10, 50))
    screen.blit(pause_count_text, (10, 90))
    screen.blit(perfect_score_count_text, (10, 130))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()
