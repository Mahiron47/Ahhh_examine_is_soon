package net.minecraft.block;

import net.minecraft.util.DyeColor;

public class StainedGlassPaneBlock extends PaneBlock implements Stainable {
	private final DyeColor color;

	public StainedGlassPaneBlock(DyeColor color, AbstractBlock.Settings settings) {
		super(settings);
		this.color = color;
		this.setDefaultState(this.stateManager.getDefaultState().with(NORTH, false).with(EAST, false).with(SOUTH, false).with(WEST, false).with(WATERLOGGED, false));
	}

	@Override
	public DyeColor getColor() {
		return this.color;
	}
}
